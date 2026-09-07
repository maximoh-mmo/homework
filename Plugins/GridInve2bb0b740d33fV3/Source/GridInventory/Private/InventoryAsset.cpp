// Copyright 2025, Solar Corp. All right reserved


#include "InventoryAsset.h"

void UInventoryAsset::AddNewSlot()
{
	UInventorySlotWrapper* newSlot = NewObject<UInventorySlotWrapper>(this);
	newSlot->InventorySlot.SlotDimension = FIntPoint(2, 2);

	if (!InventorySlots.IsEmpty())
	{
		const FInventorySlot& lastInventorySlot = InventorySlots.Last()->InventorySlot;
		
		newSlot->InventorySlot.PositionOffset = FVector2D(lastInventorySlot.PositionOffset.X + lastInventorySlot.SlotDimension.X * InventoryDefinition.GridCellSize + SpaceBetweenElements,
			lastInventorySlot.PositionOffset.Y);
	}
	
	InventorySlots.Add(newSlot);
}

void UInventoryAsset::RemoveLastSlot()
{
	if (InventorySlots.IsEmpty())
	{
		return;
	}
	
	InventorySlots.RemoveAt(InventorySlots.Num() - 1);
}

void UInventoryAsset::EmptySlots()
{
	InventorySlots.Empty();
}
