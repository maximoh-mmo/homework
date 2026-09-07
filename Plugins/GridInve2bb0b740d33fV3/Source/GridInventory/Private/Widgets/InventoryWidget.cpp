// Copyright 2025, Solar Corp. All right reserved

#include "Widgets/InventoryWidget.h"

#include "GridItem.h"
#include "GameFramework/Pawn.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Data/GridPayload.h"
#include "Interfaces/GridCharacterInterface.h"
#include "Widgets/GridItemWidget.h"
#include "Widgets/InventoryGridDrawerWidget.h"
#include "Widgets/DraggableWindow/GridWindowsLayoutWidget.h"


void UInventoryWidget::InitInventoryWidget(UInventory* InOwnerInventory, int32 InInventorySlotIndex)
{
	ItemWidgets.Empty();
	OwnerInventory = InOwnerInventory;
	InventorySlotIndex = InInventorySlotIndex;
	
	const FInventorySlot& inventorySlot = InOwnerInventory->GetInventorySlots()[InInventorySlotIndex];
	const UInventoryAsset* inventoryAsset = OwnerInventory->GetInventoryAsset();
	
	const int inventoryXSize = inventorySlot.SlotDimension.X;
	const int inventoryYSize = inventorySlot.SlotDimension.Y;
	
	SetSize(inventoryXSize * inventoryAsset->InventoryDefinition.GridCellSize, inventoryYSize * inventoryAsset->InventoryDefinition.GridCellSize);
	
	InventoryDrawerWidget->SetGridDefinition(inventoryAsset, inventorySlot.SlotDimension);

	for (auto& [item, TopLeftTile] : inventorySlot.GetItems())
	{
		if (IsValid(item))
		{
			AddItem(item, TopLeftTile, InInventorySlotIndex);
		}
	}

	OwnerInventory->OnItemAdded.AddUniqueDynamic(this, &UInventoryWidget::AddItem);
	OwnerInventory->OnItemRemoved.AddUniqueDynamic(this, &UInventoryWidget::OnItemRemoved);
}

void UInventoryWidget::SetSize(float InWidth, float InHeight)
{
	RootSizeBox->SetWidthOverride(InWidth);
	RootSizeBox->SetHeightOverride(InHeight);
}

void UInventoryWidget::AddItem(UGridItem* InItem, const FIntPoint& InTopLeftPoint, int32 InSlotIndex)
{
	if (InSlotIndex != InventorySlotIndex)
	{
		return;
	}

	const UInventoryAsset* inventoryAsset = OwnerInventory->GetInventoryAsset();
	
	UGridItemWidget* itemWidget = CreateWidget<UGridItemWidget>(this, GridItemWidgetClass);
	itemWidget->SetupItem(InItem, inventoryAsset);
	itemWidget->OnDragDropCreated.AddUniqueDynamic(this, &UInventoryWidget::OnItemCreatedPayload);
	ItemWidgets.Add(itemWidget);

	UCanvasPanelSlot* itemAsCanvasSlot = InventoryCanvas->AddChildToCanvas(itemWidget);
	itemAsCanvasSlot->SetAutoSize(true);
	itemAsCanvasSlot->SetPosition(FVector2D((float)InTopLeftPoint.X * (float)inventoryAsset->InventoryDefinition.GridCellSize, (float)InTopLeftPoint.Y * (float)inventoryAsset->InventoryDefinition.GridCellSize));
}

void UInventoryWidget::OnItemRemoved(UGridItem* InItem)
{
	for (int32 i = 0; i < ItemWidgets.Num(); ++i)
	{
		UGridItemWidget* currentWidget = ItemWidgets[i];
		if (currentWidget->GetItem() == InItem)
		{
			currentWidget->RemoveFromParent();
			ItemWidgets.RemoveAt(i);
			return;
		}
	}
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

FVector2D UInventoryWidget::ClampDraggedItemSize(const FVector2D& InDraggedItemSize, const FIntPoint& InDraggedItemTopLeftTile, UGridItem* InDraggedItem) const
{
	const UInventoryAsset* inventoryAsset = OwnerInventory->GetInventoryAsset();
	const FIntPoint& slotDimensions = OwnerInventory->GetInventorySlots()[InventorySlotIndex].SlotDimension;
	const FIntPoint& itemDimensions = InDraggedItem->GetDimensions();
	const FIntPoint clampedDimensions = slotDimensions - InDraggedItemTopLeftTile;

	float maxX = InDraggedItemSize.X;
	float maxY = InDraggedItemSize.Y;
	
	if (itemDimensions.X > clampedDimensions.X)
	{
		maxX = clampedDimensions.X * inventoryAsset->InventoryDefinition.GridCellSize;
	}
	if (itemDimensions.Y > clampedDimensions.Y)
	{
		maxY = clampedDimensions.Y * inventoryAsset->InventoryDefinition.GridCellSize;
	}
	
	return FVector2D(maxX, maxY);
}

void UInventoryWidget::StartItemSplitting(UGridPayload* InGridPayload, const FGridWindowSpecification& InWindowSpec,
	const FIntPoint& InDraggedItemTopLeftTile)
{
	APawn* playerPawn = GetOwningPlayerPawn();
	if (!ensureAlwaysMsgf(GetOwningPlayerPawn()->Implements<UGridCharacterInterface>(), TEXT("Inventory widget owner, doesn't implement IGridCharacterInterface")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(IsValid(InGridPayload), TEXT("Attempt to split an item with invalid grid payload")))
	{
		return;
	}
	
	if (!ensureAlwaysMsgf(!IsValid(SplittingItemPayload), TEXT("SplittingItemPayload is valid, seems like we have 2 or more splitting logic at the same time, whihc is not supported")))
	{
		return;
	}
	
	UGridWindowsLayoutWidget* layoutWidget = IGridCharacterInterface::Execute_GetGridWindowsLayoutWidget(playerPawn);
	FPopupItemSplittingCountDelegate splittingCountSelectedCallback;
	splittingCountSelectedCallback.BindDynamic(this, &UInventoryWidget::OnItemSplittingCountSelected);
	
	FPopupCallbackDelegate splittingCanceledCallback;
	splittingCanceledCallback.BindDynamic(this, &UInventoryWidget::OnItemSplittingCanceled);
	
	layoutWidget->ShowSplitItemPopupWindow(InWindowSpec, InGridPayload->GridItem, splittingCountSelectedCallback, splittingCanceledCallback);

	SplittingItemPayload = InGridPayload;
	SplittingItemTopLeftTile = InDraggedItemTopLeftTile;
}

void UInventoryWidget::StartItemTransferring(UGridItem* InStacksReceiver, UGridPayload* InGridPayload,
	const FGridWindowSpecification& InWindowSpec)
{
	APawn* playerPawn = GetOwningPlayerPawn();
	if (!ensureAlwaysMsgf(GetOwningPlayerPawn()->Implements<UGridCharacterInterface>(), TEXT("Inventory widget owner, doesn't implement IGridCharacterInterface")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(IsValid(InGridPayload), TEXT("Attempt to split an item with invalid grid payload")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(!IsValid(SplittingItemPayload), TEXT("SplittingItemPayload is valid, seems like we have 2 or more splitting logic at the same time, whihc is not supported")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(InStacksReceiver->IsStackable(), TEXT("InStacksReceiver isn't stackable")) || !ensureAlwaysMsgf(InGridPayload->GridItem->IsStackable(), TEXT("gridItem isn't stackable")))
	{
		return;
	}

	UGridWindowsLayoutWidget* layoutWidget = IGridCharacterInterface::Execute_GetGridWindowsLayoutWidget(playerPawn);
	FPopupItemSplittingCountDelegate splittingCountSelectedCallback;
	splittingCountSelectedCallback.BindDynamic(this, &UInventoryWidget::OnItemTransferringCountSelected);
	
	FPopupCallbackDelegate splittingCanceledCallback;
	splittingCanceledCallback.BindDynamic(this, &UInventoryWidget::OnItemTransferringCanceled);

	const int32 maxCount = FMath::Min(InGridPayload->GridItem->GetCurrentStacks(), InStacksReceiver->GetMaxStacks() - InStacksReceiver->GetCurrentStacks());
	
	layoutWidget->ShowTransferringItemPopupWindow(InWindowSpec, maxCount, splittingCountSelectedCallback, splittingCanceledCallback);

	SplittingItemPayload = InGridPayload;
	StackReceiverItem = InStacksReceiver;
}

void UInventoryWidget::OnItemCreatedPayload(UGridPayload* InPayload)
{
	InPayload->SourceInventory = OwnerInventory;
	InPayload->SourceInventorySlotIndex = InventorySlotIndex;
}

void UInventoryWidget::OnItemSplittingCountSelected(int32 InSplittingCount)
{
	UGridItem* item = SplittingItemPayload->GridItem;
	
	// All stacks were selected, so we need to remove item from source inventory
	if (InSplittingCount == item->GetCurrentStacks())
	{
		IGridItemOwner::Execute_RemoveItemCommand(item->GetItemOwnerInterfaceObject(), item);

		OwnerInventory->AddItemInSlot(item, InventorySlotIndex, SplittingItemTopLeftTile);
		SplittingItemPayload->GridItemWidget->RemoveFromParent();
	}
	else
	{
		item->SetCurrentStacks(item->GetCurrentStacks() - InSplittingCount);
		OwnerInventory->SplitItemToSlot(item, InventorySlotIndex, SplittingItemTopLeftTile, InSplittingCount);
		SplittingItemPayload->ResetItemInitialRotation();
		SplittingItemPayload->GridItemWidget->UpdateItemDisplay();
	}
	
	OnItemSplittingCanceled();
}

void UInventoryWidget::OnItemTransferringCountSelected(int32 InTransferringCount)
{
	bool wasFullTransfer;
	OwnerInventory->TransferItemsStack(StackReceiverItem, SplittingItemPayload->GridItem, InTransferringCount, wasFullTransfer);

	if (wasFullTransfer)
	{
		IGridItemOwner::Execute_RemoveItemCommand(SplittingItemPayload->GridItem->GetItemOwnerInterfaceObject(), SplittingItemPayload->GridItem);
		SplittingItemPayload->GridItemWidget->RemoveFromParent();
	}

	OnItemTransferringCanceled();
}

void UInventoryWidget::OnItemSplittingCanceled()
{
	SplittingItemPayload = nullptr;
}

void UInventoryWidget::OnItemTransferringCanceled()
{
	SplittingItemPayload = nullptr;
	StackReceiverItem = nullptr;
}
