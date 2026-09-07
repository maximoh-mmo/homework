// Copyright 2025, Solar Corp. All right reserved

#include "InventoryAssetEditorApp.h"

#include "GridInventoryEditor.h"
#include "InventoryAssetAppMode.h"
#include "InventoryAsset.h"
#include "SlateWidgets/GridDrawer.h"


InventoryAssetEditorApp::~InventoryAssetEditorApp()
{
	GEditor->UnregisterForUndo(this);
}

void InventoryAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void InventoryAssetEditorApp::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::UnregisterTabSpawners(InTabManager);
}

void InventoryAssetEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	mWorkingAsset = Cast<UInventoryAsset>(InObject);
	// undo/redo support
	mWorkingAsset->SetFlags(RF_Transactional);
	if (!mWorkingAsset->InventorySlots.IsEmpty())
	{
		mSelectedInventorySlot = mWorkingAsset->InventorySlots[0];
	}
	
	TArray<UObject*> objectsToEdit;
	objectsToEdit.Add(InObject);

	GEditor->RegisterForUndo(this);

	InitAssetEditor(Mode, InitToolkitHost, TEXT("InventoryAssetEditor"), FTabManager::FLayout::NullLayout, true, true, objectsToEdit);
	
	AddApplicationMode(GridInventory::InventoryAssetAppMode, MakeShareable(new InventoryAssetAppMode(SharedThis(this))));
	SetCurrentMode(GridInventory::InventoryAssetAppMode);
	
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());

	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("CustomTools");

			ToolbarBuilder.AddToolBarButton(
			FUIAction(FExecuteAction::CreateLambda([]()
			{
			})),
			NAME_None,
			FText::FromString("Custom"),
			FText::FromString("Click to do something"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Info")
			);

			ToolbarBuilder.EndSection();
		})
	);

	AddToolbarExtender(ToolbarExtender);
	RegenerateMenusAndToolbars();
}

TSharedRef<SWidget> InventoryAssetEditorApp::CreateGridInventoriesContainer()
{
	mGridInventoriesRootOverlay = SAssignNew(mGridInventoriesRootOverlay, SOverlay);
	FillInventoriesOverlay();
	
	return mGridInventoriesRootOverlay.ToSharedRef();
}

TSharedRef<SWidget> InventoryAssetEditorApp::CreatePropertiesWidget()
{
	FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs detailsViewArgs;
	detailsViewArgs.bAllowSearch = false;
	detailsViewArgs.bHideSelectionTip = true;
	detailsViewArgs.bLockable = false;
	detailsViewArgs.bSearchInitialKeyFocus = true;
	detailsViewArgs.bUpdatesFromSelection = false;
	detailsViewArgs.bShowOptions = true;
	detailsViewArgs.bShowModifiedPropertiesOption = false;
	detailsViewArgs.bShowScrollBar = false;
	detailsViewArgs.NotifyHook = this;
	
	// creating details view for inventory asset
	TSharedPtr<IDetailsView> detailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
	detailsView->SetObject(GetWorkingAsset());

	// Creating details view for selected inventory slot
	mSelectedInventorySlotDetailsView = propertyEditorModule.CreateDetailView(detailsViewArgs);
	mSelectedInventorySlotDetailsView->SetObject(mSelectedInventorySlot.IsValid() ? mSelectedInventorySlot.Get() : nullptr);
	
	return SNew(SVerticalBox)
	+SVerticalBox::Slot()
	.AutoHeight()
	.Padding(5.f)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SButton)
				.OnClicked(this, &InventoryAssetEditorApp::OnAddNewSlotClicked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Add Slot"))
					.Justification(ETextJustify::Center) 
				]
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.Padding(5.f, 0.f, 0.f, 0.f)
			[
				SNew(SButton)
				.OnClicked(this, &InventoryAssetEditorApp::OnRemoveSelectedSlotClicked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Remove Selected Slot"))
					.Justification(ETextJustify::Center) 
				]
			]
		]
		+SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(0.f, 5.f, 0.f, 0.f)
		[
			SNew(SButton)
			.OnClicked(this, &InventoryAssetEditorApp::OnRemoveAllSlotsClicked)
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Remove All Slots"))
				.Justification(ETextJustify::Center) 
			]
		]
	]
	+ SVerticalBox::Slot()
	.FillHeight(1.0f)
	.HAlign(HAlign_Fill)
	[
		detailsView.ToSharedRef()
	]
	+ SVerticalBox::Slot()
	.FillHeight(1.0f)
	.HAlign(HAlign_Fill)
	[
		mSelectedInventorySlotDetailsView.ToSharedRef()
	];
}

UInventoryAsset* InventoryAssetEditorApp::GetWorkingAsset() const
{
	return mWorkingAsset.IsValid() ? mWorkingAsset.Get() : nullptr;
}

void InventoryAssetEditorApp::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged)
{
	FNotifyHook::NotifyPostChange(PropertyChangedEvent, PropertyThatChanged);

	if (mGridInventoriesRootOverlay->GetChildren()->Num() == 0)
	{
		return;
	}
	
	FillInventoriesOverlay();
}

void InventoryAssetEditorApp::FillInventoriesOverlay()
{
	mGridInventoriesRootOverlay->ClearChildren();
	for (int32 i = 0; i < mWorkingAsset->InventorySlots.Num(); ++i)
	{
		const FInventorySlot& inventorySlot = mWorkingAsset->InventorySlots[i]->InventorySlot;
		const float width = inventorySlot.SlotDimension.X * mWorkingAsset->InventoryDefinition.GridCellSize;
		const float height = inventorySlot.SlotDimension.Y * mWorkingAsset->InventoryDefinition.GridCellSize;
		
		mGridInventoriesRootOverlay->AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(inventorySlot.PositionOffset.X, inventorySlot.PositionOffset.Y, 0, 0)
		[
			SNew(SBox)
			.WidthOverride(width)
			.HeightOverride(height)
			[
				SNew(SGridDrawer)
				.GridDimensions(inventorySlot.SlotDimension)
				.GridSize(mWorkingAsset->InventoryDefinition.GridCellSize)
				.GridColor(mWorkingAsset->InventoryDefinition.GridLineColor)
				.IsSelected_Lambda([this, i]()
				{
					return mSelectedInventorySlotIndex == i;
				})
				.OnMouseButtonDown_Lambda([this, i](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
				{
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
					{
						mSelectedInventorySlot = mWorkingAsset->InventorySlots[i];
						mSelectedInventorySlotIndex = i;
						mSelectedInventorySlotDetailsView->SetObject(mSelectedInventorySlot.IsValid() ? mSelectedInventorySlot.Get() : nullptr);

						// Update layout so the right object is higlighted
						FillInventoriesOverlay();
						return FReply::Handled();
					}

					return FReply::Unhandled();
				})
			]
		];
	}
}

void InventoryAssetEditorApp::SelectLastInventorySlot()
{
	if (!mWorkingAsset->InventorySlots.IsEmpty())
	{
		mSelectedInventorySlot = mWorkingAsset->InventorySlots.Last();
		mSelectedInventorySlotIndex = mWorkingAsset->InventorySlots.Num() - 1;
		mSelectedInventorySlotDetailsView->SetObject(mSelectedInventorySlot.IsValid() ? mSelectedInventorySlot.Get() : nullptr);
	}
	else
	{
		mSelectedInventorySlotIndex = 0;
		mSelectedInventorySlot = nullptr;
		mSelectedInventorySlotDetailsView->SetObject(nullptr);
	}
		
	FillInventoriesOverlay();
}

FReply InventoryAssetEditorApp::OnAddNewSlotClicked()
{
	const FScopedTransaction transaction(FText::FromString("Adding new slot"));
	
	mWorkingAsset->Modify();
	mWorkingAsset->AddNewSlot();

	// undo/redo support
	mWorkingAsset->InventorySlots.Last()->SetFlags(RF_Transactional);

	SelectLastInventorySlot();
	FillInventoriesOverlay();

	return FReply::Handled();
}

FReply InventoryAssetEditorApp::OnRemoveSelectedSlotClicked()
{
	if (mWorkingAsset->InventorySlots.IsEmpty())
	{
		return FReply::Handled();
	}

	const FScopedTransaction transaction(FText::FromString("Removing selected slot"));
	
	mWorkingAsset->Modify();
	mWorkingAsset->InventorySlots.RemoveAt(mSelectedInventorySlotIndex);
	
	SelectLastInventorySlot();
	FillInventoriesOverlay();

	return FReply::Handled();
}

FReply InventoryAssetEditorApp::OnRemoveAllSlotsClicked()
{
	if (mWorkingAsset->InventorySlots.IsEmpty())
	{
		return FReply::Handled();
	}

	const FScopedTransaction transaction(FText::FromString("Removing all slots"));
	
	mWorkingAsset->Modify();
	mWorkingAsset->EmptySlots();

	SelectLastInventorySlot();
	FillInventoriesOverlay();

	return FReply::Handled();
}

FName InventoryAssetEditorApp::GetToolkitFName() const
{
	return FName(TEXT("InventoryAssetEditorApp"));
}

FText InventoryAssetEditorApp::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("InventoryAssetEditorApp"));
}

FString InventoryAssetEditorApp::GetWorldCentricTabPrefix() const
{
	return TEXT("InventoryAssetEditorApp");
}

FLinearColor InventoryAssetEditorApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

FString InventoryAssetEditorApp::GetDocumentationLink() const
{
	// TODO: add
	return "";
}

void InventoryAssetEditorApp::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingStarted(Toolkit);
}

void InventoryAssetEditorApp::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingFinished(Toolkit);
}

void InventoryAssetEditorApp::PostUndo(bool bSuccess)
{
	HandleUndoRedo();
}

void InventoryAssetEditorApp::PostRedo(bool bSuccess)
{
	HandleUndoRedo();
}

void InventoryAssetEditorApp::HandleUndoRedo()
{
	SelectLastInventorySlot();
}
