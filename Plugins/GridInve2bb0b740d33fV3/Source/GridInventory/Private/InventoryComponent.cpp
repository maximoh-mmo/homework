// Copyright 2025, Solar Corp. All right reserved

#include "InventoryComponent.h"

#include "GridItem.h"
#include "Inventory.h"
#include "InventoryAsset.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UInventory* UInventoryComponent::GetInventory() const
{
	return Inventory;
}

void UInventoryComponent::InitInventory()
{
	if (!ensureAlwaysMsgf(IsValid(InventoryAsset), TEXT("InventoryAsset is null, assign an asset for inventory")))
	{
		return;
	}
	
	Inventory = NewObject<UInventory>();
	Inventory->InitInventory(InventoryAsset);
	
	for (UGridItem* item : InitialItems)
	{
		if (!ensureAlwaysMsgf(IsValid(item), TEXT("Arrempt to add an invalid item, check your InitialItems")))
		{
			continue;
		}

		item->InitItem();
		Inventory->TryAddItem(item);
	}
}
