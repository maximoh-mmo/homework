// Copyright 2025, Solar Corp. All right reserved


#include "Character/PlayerInventoryEquipWidget.h"

#include "Inventory.h"
#include "InventoryComponent.h"
#include "Character/GridInventoryCharacter.h"
#include "Components/NamedSlot.h"
#include "Equip/EquipAreaWidget.h"
#include "Widgets/InventoryLayoutWidget.h"


void UPlayerInventoryEquipWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AGridInventoryCharacter* player = Cast<AGridInventoryCharacter>(GetOwningPlayerPawn());
	if (IsValid(player))
	{
		UInventoryComponent* playerPocketsInventory = player->GetPlayerPocketsInventoryComponent();
		AddInventoryToPocketsSlot(playerPocketsInventory->GetInventory());
	}
}

const TArray<UInventory*>& UPlayerInventoryEquipWidget::GetEquippedInventories() const
{
	return EquippedInventories;
}

void UPlayerInventoryEquipWidget::AddInventoryToPocketsSlot(UInventory* InInventory)
{
	AddLayoutToSlot(InInventory, PocketsInventorySlot);
}

void UPlayerInventoryEquipWidget::AddInventoryToBackpackSlot(UInventory* InInventory)
{
	AddLayoutToSlot(InInventory, BackpackInventorySlot);
}

void UPlayerInventoryEquipWidget::AddInventoryToRigSlot(UInventory* InInventory)
{
	AddLayoutToSlot(InInventory, RigInventorySlot);
}

void UPlayerInventoryEquipWidget::AddLayoutToSlot(UInventory* InInventory, UNamedSlot* InInventorySlot)
{
	UInventoryLayoutWidget* inventoryLayout = CreateWidget<UInventoryLayoutWidget>(GetOwningPlayer(), InventoryLayoutClass);
	inventoryLayout->InitLayout(InInventory);
	InInventorySlot->AddChild(inventoryLayout);
	
	EquippedInventories.Add(InInventory);
}

bool UPlayerInventoryEquipWidget::TryEquipItemInAnyAvailableSlotOrInventory(UGridItem* InItem)
{
	if (RigEquipArea->TryEquipItem(InItem))
	{
		return true;
	}

	if (BackpackEquipArea->TryEquipItem(InItem))
	{
		return true;
	}

	for (UInventory* inventory : EquippedInventories)
	{
		if (inventory->TryAddItem(InItem))
		{
			return true;
		}
	}

	return false;
}

void UPlayerInventoryEquipWidget::RemoveEquippedInventory(UInventory* InEquippedInventory)
{
	if (!ensureAlwaysMsgf(EquippedInventories.Contains(InEquippedInventory), TEXT("Attempt to remove inventory that is not inside EquippedInventories")))
	{
		return;
	}
	
	EquippedInventories.Remove(InEquippedInventory);
}