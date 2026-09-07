// Copyright 2025, Solar Corp. All right reserved

#include "InventoryAssetPrimaryTabFactory.h"

#include "GridInventoryEditor.h"
#include "InventoryAssetEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"


InventoryAssetPrimaryTabFactory::InventoryAssetPrimaryTabFactory(TSharedPtr<InventoryAssetEditorApp> App)
	: FWorkflowTabFactory(GridInventory::InventoryAssetPrimaryTabName, App), mApp(App)
{
	TabLabel = FText::FromString(TEXT("Inventory View"));
	ViewMenuDescription = FText::FromString(TEXT("Displays a primary view for inventory asset"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the primary inventory view"));
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports");
}

TSharedRef<SWidget> InventoryAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<InventoryAssetEditorApp> app = mApp.Pin();

	return SNew(SScrollBox)
	+SScrollBox::Slot()
	.VAlign(VAlign_Fill)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.Padding(10.f)
		[
			app->CreateGridInventoriesContainer()
		]
	];
}

FText InventoryAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A primary view for displaying inventory layout"));
}
