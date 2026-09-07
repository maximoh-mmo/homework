// Copyright 2025, Solar Corp. All right reserved


#include "Inventory.h"

#include "GridItem.h"
#include "InventoryAsset.h"
#include "Data/GridPayload.h"

void FInventoryGridCell::Reset()
{
	AssociatedItem = nullptr;
}

bool FInventoryGridCell::IsEmpty() const
{
	return !IsValid(AssociatedItem);
}

bool FInventorySlot::CanFitItem(const FIntPoint& ItemDimension) const
{
	if (ItemDimension.X > SlotDimension.X || ItemDimension.Y > SlotDimension.Y)
	{
		return false;
	}

	return false;
}

const TArray<TPair<UGridItem*, FIntPoint>>& FInventorySlot::GetItems() const
{
	return ItemToTopLeftTileList;
}

UGridItem* FInventorySlot::GetItemInCell(const FIntPoint& InCellPos)
{
	FInventoryGridCell* gridCell = GetGridCellInternal(InCellPos.X, InCellPos.Y);
	if (gridCell != nullptr)
	{
		return gridCell->AssociatedItem;
	}

	return nullptr;
}

void FInventorySlot::InitSlot()
{
	SlotGridCells.Reserve(SlotDimension.X * SlotDimension.Y);
	SlotGridCells.Init({}, SlotDimension.X * SlotDimension.Y);
}

bool FInventorySlot::CanFitItem(UGridItem* InItem, FIntPoint& TopLeftTilePosOut, FIntPoint StartingPos, bool StopIfFirstTileOccupied, bool IgnoreSelfItem)
{
	int32 startingX = StartingPos.X == -1 ? 0 : StartingPos.X;
	int32 startingY = StartingPos.Y == -1 ? 0 : StartingPos.Y;

	FInventoryGridCell* cell = GetGridCellInternal(startingX, startingY);
	bool shouldSkip = IgnoreSelfItem && cell->AssociatedItem == InItem;
	if (StopIfFirstTileOccupied && !cell->IsEmpty() && !shouldSkip)
	{
		return false;
	}
	
	const FIntPoint itemDimensions = InItem->GetDimensions();
	if (itemDimensions.X + startingX > SlotDimension.X || itemDimensions.Y + startingY > SlotDimension.Y)
	{
		return false;
	}
	
	// iterate foreach horizontal cells and try to fit an item
	
	int32 foundStartingX = startingX;
	int32 foundStartingY = startingY;

	int32 foundTileCount = 0;

	const int32 maxYIteration = StopIfFirstTileOccupied ? startingY + itemDimensions.Y : SlotDimension.Y;
	const int32 maxXIteration = StopIfFirstTileOccupied ? startingX + itemDimensions.X : SlotDimension.X;
	
	for (int32 y = startingY; y < maxYIteration; ++y)
	{
		if (foundTileCount == 0)
		{
			foundStartingY = y;
		}

		int32 currentTileCountOnCurrentRow = 0;
		for (int32 x = foundStartingX; x < maxXIteration; ++x)
		{
			if (foundTileCount == 0)
			{
				foundStartingX = x;
			}
			
			FInventoryGridCell* currentCell = GetGridCellInternal(x, y);
			shouldSkip = IgnoreSelfItem && currentCell->AssociatedItem == InItem;
			if (!currentCell->IsEmpty() && !shouldSkip)
			{
				foundStartingX = startingX;
				foundTileCount = 0;
				currentTileCountOnCurrentRow = 0;
				continue;
			}

			foundTileCount++;
			currentTileCountOnCurrentRow++;

			// Item could be places
			if (foundTileCount == itemDimensions.X * itemDimensions.Y)
			{
				TopLeftTilePosOut = FIntPoint(foundStartingX, foundStartingY);

				return true;
			}

			// we found all required tiles on current row
			if (currentTileCountOnCurrentRow == itemDimensions.X)
			{
				break;
			}

			// handle case when current row doesn't have enough element to fit and item
			if (x == SlotDimension.X - 1)
			{
				foundStartingX = startingX;
				foundTileCount = 0;
				break;
			}
		}
	}

	TopLeftTilePosOut.X = -1;
	TopLeftTilePosOut.Y = -1;
	return false;
}

bool FInventorySlot::TryAddItem(UGridItem* InItem, FIntPoint& TopLeftTilePosOut)
{
	if (CanFitItem(InItem, TopLeftTilePosOut))
	{
		AddItem(InItem, TopLeftTilePosOut);
		return true;
	}

	return false;
}

bool FInventorySlot::RemoveItem(UGridItem* InItem)
{
	if (!IsValid(InItem))
	{
		return false;
	}

	bool bWasItemFound = false;
	for (int32 i = 0; i < ItemToTopLeftTileList.Num(); ++i)
	{
		auto&[item, topLeftPoint] = ItemToTopLeftTileList[i];
		if (item == InItem)
		{
			bWasItemFound = true;
			ItemToTopLeftTileList.RemoveAt(i);
			break;
		}
	}

	if (!bWasItemFound)
	{
		return false;
	}
	
	for (int32 y = 0; y < SlotDimension.Y; ++y)
	{
		for (int x = 0; x < SlotDimension.X; ++x)
		{
			FInventoryGridCell* gridCell = GetGridCellInternal(x, y);
			if (gridCell->AssociatedItem == InItem)
			{
				gridCell->Reset();
			}
		}
	}

	return true;
}

FInventoryGridCell* FInventorySlot::GetGridCellInternal(int32 X, int32 Y)
{
	if (X >= SlotDimension.X || Y >= SlotDimension.Y)
	{
		ensureAlways(false);
		return nullptr;
	}

	return &SlotGridCells[Y * SlotDimension.X + X];
}

void FInventorySlot::AddItem(UGridItem* InItem, const FIntPoint& TopLeftTilePos)
{
	ItemToTopLeftTileList.Add({InItem, TopLeftTilePos});

	const FIntPoint itemDimensions = InItem->GetDimensions();
	for (int32 y = 0; y < itemDimensions.Y; ++y)
	{
		for (int x = 0; x < itemDimensions.X; ++x)
		{
			FInventoryGridCell* gridCell = GetGridCellInternal(x + TopLeftTilePos.X, y + TopLeftTilePos.Y);
			gridCell->AssociatedItem = InItem;
		}
	}
}

void UInventory::RemoveItemCommand_Implementation(UGridItem* Item)
{
	RemoveItem(Item);
}

TArray<TSubclassOf<UGridItemActionBase>> UInventory::GetOwnerActionClasses_Implementation() const
{
	if (!IsValid(InventoryAsset))
	{
		return {};
	}

	return InventoryAsset->InventoryDefinition.ActionsToGrant;
}

void UInventory::InitInventory(UInventoryAsset* InAsset)
{
	InventoryAsset = InAsset;
	
	for (int32 i = 0; i < InAsset->InventorySlots.Num(); ++i)
	{
		UInventorySlotWrapper* slotWrapper = InAsset->InventorySlots[i];
		InventorySlots.Emplace(slotWrapper->InventorySlot);
		InventorySlots[i].InitSlot();
	}
}

const TArray<FInventorySlot>& UInventory::GetInventorySlots() const
{
	return InventorySlots;
}

const UInventoryAsset* UInventory::GetInventoryAsset() const
{
	return InventoryAsset;
}

bool UInventory::TryAddItem(UGridItem* InItem)
{
	for (UGridItem* item : GetAllItems())
	{
		// we should not add an item that is already in the inventory
		if (item == InItem)
		{
			ensureAlwaysMsgf(false, TEXT("Attempt to add an item that is already in inventory"));
			return false;
		}
	}
	
	auto addingLambda = [this](UGridItem* InItem)
	{
		FIntPoint topLeftTilePos;
		for (int32 i = 0; i < InventorySlots.Num(); ++i)
		{
			FInventorySlot& slot = InventorySlots[i];
			if (slot.TryAddItem(InItem, topLeftTilePos))
			{
				InItem->SetItemOwnerInterfaceObject(this);
				InItem->SetGridCellSize(InventoryAsset->InventoryDefinition.GridCellSize);
				InItem->GenerateIcon();
				if (OnItemAdded.IsBound())
				{
					OnItemAdded.Broadcast(InItem, topLeftTilePos, i);
				}
				
				return true;
			}
		}

		return false;
	};

	if (addingLambda(InItem))
	{
		return true;
	}

	// Try put rotated item.
	InItem->SetIsRotated(!InItem->GetIsRotated());
	if (addingLambda(InItem))
	{
		return true;
	}

	InItem->SetIsRotated(!InItem->GetIsRotated());
	return false;
}

bool UInventory::CanAddItem(UGridItem* InItem)
{
	for (UGridItem* item : GetAllItems())
	{
		// We cannot add an item that is already in inventory
		if (item == InItem)
		{
			return false;
		}
	}
	
	auto addingLambda = [this](UGridItem* InItem)
	{
		FIntPoint topLeftTilePos;
		for (int32 i = 0; i < InventorySlots.Num(); ++i)
		{
			FInventorySlot& slot = InventorySlots[i];
			if (slot.CanFitItem(InItem, topLeftTilePos))
			{
				return true;
			}
		}

		return false;
	};

	if (addingLambda(InItem))
	{
		return true;
	}

	InItem->SetIsRotated(!InItem->GetIsRotated());
	if (addingLambda(InItem))
	{
		InItem->SetIsRotated(!InItem->GetIsRotated());
		return true;
	}

	InItem->SetIsRotated(!InItem->GetIsRotated());
	return false;
}

UGridItem* UInventory::TryGetItemFromTopLeftPos(int32 InSlotIndex, const FIntPoint& InTopLeftTilePos)
{
	if (!ensureAlways(InSlotIndex < InventorySlots.Num()))
	{
		return nullptr;
	}

	for (auto& [item, pos] : InventorySlots[InSlotIndex].GetItems())
	{
		if (InTopLeftTilePos.X >= pos.X && InTopLeftTilePos.X < pos.X + item->Dimensions.X
			&& InTopLeftTilePos.Y >= pos.Y && InTopLeftTilePos.Y < pos.Y + item->Dimensions.Y)
		{
			return item;
		}
	}

	return nullptr;
}

UGridItem* UInventory::TryGetItemFromPos(int32 InSlotIndex, const FIntPoint& InTilePos)
{
	if (!ensureAlways(InSlotIndex < InventorySlots.Num()))
	{
		return nullptr;
	}

	return InventorySlots[InSlotIndex].GetItemInCell(InTilePos);
}

bool UInventory::CanFitItem(UGridItem* InItem, int32 InSlotIndex, FIntPoint InStartingTile, bool StopIfFirstTileOccupied, bool IgnoreSelfItem, FIntPoint& TopLeftTilePosOut)
{
	if (!ensureAlways(InSlotIndex < InventorySlots.Num()))
	{
		return false;
	}

	if (GetOuter() == InItem)
	{
		return false;
	}

	return InventorySlots[InSlotIndex].CanFitItem(InItem, TopLeftTilePosOut, InStartingTile, StopIfFirstTileOccupied, IgnoreSelfItem);
}

void UInventory::AddItemInSlot(UGridItem* InItem, int32 InDestinationSlotIndex, FIntPoint InDestinationTopLeftTile)
{
	InventorySlots[InDestinationSlotIndex].AddItem(InItem, InDestinationTopLeftTile);
	InItem->SetItemOwnerInterfaceObject(this);
	if (OnItemAdded.IsBound())
	{
		OnItemAdded.Broadcast(InItem, InDestinationTopLeftTile, InDestinationSlotIndex);
	}
}

void UInventory::SplitItemToSlot(UGridItem* InItem, int32 InDestinationSlotIndex,
	FIntPoint InDestinationTopLeftTile, int32 InStacks)
{
	UGridItem* splitItem = DuplicateObject(InItem, InItem->GetOuter());
	splitItem->SetCurrentStacks(InStacks);
	
	AddItemInSlot(splitItem, InDestinationSlotIndex, InDestinationTopLeftTile);
}

void UInventory::StackItems(UGridItem* InStacksReceiver, UGridItem* InStackGiver, bool& OutWasFullTransfer)
{
	if (!ensureAlwaysMsgf(IsValid(InStacksReceiver), TEXT("InStacksReceiver is null")) || !ensureAlwaysMsgf(IsValid(InStackGiver), TEXT("InStackGiver is null")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(InStacksReceiver->IsStackable(), TEXT("InStacksReceiver isn't stackable")) || !ensureAlwaysMsgf(InStackGiver->IsStackable(), TEXT("InStackGiver isn't stackable")))
	{
		return;
	}

	int32 stacksToGive = InStackGiver->GetCurrentStacks();
	StackItemsInternal(InStacksReceiver, InStackGiver, stacksToGive, OutWasFullTransfer);
}

void UInventory::TransferItemsStack(UGridItem* InStacksReceiver, UGridItem* InStackGiver, int32 InStackCount,
	bool& OutWasFullTransfer)
{
	if (!ensureAlwaysMsgf(IsValid(InStacksReceiver), TEXT("InStacksReceiver is null")) || !ensureAlwaysMsgf(IsValid(InStackGiver), TEXT("InStackGiver is null")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(InStacksReceiver->IsStackable(), TEXT("InStacksReceiver isn't stackable")) || !ensureAlwaysMsgf(InStackGiver->IsStackable(), TEXT("InStackGiver isn't stackable")))
	{
		return;
	}
	
	if (!ensureAlwaysMsgf(InStackCount <= InStackGiver->GetCurrentStacks(), TEXT("Attempt to give more stacks than InStackGiver has")))
	{
		return;
	}
	
	StackItemsInternal(InStacksReceiver, InStackGiver, InStackCount, OutWasFullTransfer);
}

void UInventory::StackItemsInternal(UGridItem* InStacksReceiver, UGridItem* InStackGiver, int32 InStackCout,
	bool& OutWasFullTransfer)
{
	if (InStackGiver->GetCurrentStacks() != InStackCout)
	{
		InStackGiver->SetCurrentStacks(InStackGiver->GetCurrentStacks() - InStackCout);
		InStacksReceiver->SetCurrentStacks(InStacksReceiver->GetCurrentStacks() + InStackCout);
		InStacksReceiver->RequestWidgetUpdate();
		InStackGiver->RequestWidgetUpdate();

		OutWasFullTransfer = false;
	}
	else
	{
		if (InStacksReceiver->GetCurrentStacks() + InStackCout <= InStacksReceiver->GetMaxStacks())
		{
			InStacksReceiver->SetCurrentStacks(InStacksReceiver->GetCurrentStacks() + InStackCout);
			InStacksReceiver->RequestWidgetUpdate();
			OutWasFullTransfer = true;
		}
		else
		{
			const int32 maxStacksToReceive = InStacksReceiver->GetMaxStacks() - InStacksReceiver->GetCurrentStacks();
			InStacksReceiver->SetCurrentStacks(InStacksReceiver->GetCurrentStacks() + maxStacksToReceive);
			InStackGiver->SetCurrentStacks(InStackGiver->GetCurrentStacks() - maxStacksToReceive);
			InStacksReceiver->RequestWidgetUpdate();
			InStackGiver->RequestWidgetUpdate();
			
			OutWasFullTransfer = false;
		}
	}
}

void UInventory::RemoveItemFromSlot(UGridItem* InItem, int32 InSourceSlotIndex)
{
	InventorySlots[InSourceSlotIndex].RemoveItem(InItem);
	if (OnItemRemoved.IsBound())
	{
		OnItemRemoved.Broadcast(InItem);
	}
}

void UInventory::RemoveItem(UGridItem* InItem)
{
	for (FInventorySlot& slot : InventorySlots)
	{
		if (slot.RemoveItem(InItem))
		{
			if (OnItemRemoved.IsBound())
			{
				OnItemRemoved.Broadcast(InItem);
			}
			return;
		}
	}
}

TArray<UGridItem*> UInventory::GetAllItems() const
{
	TArray<UGridItem*> items;
	
	for (const FInventorySlot& slot : InventorySlots)
	{
		for (auto& [item, topLeftPos] : slot.GetItems())
		{
			items.Add(item);
		}
	}

	return items;
}
