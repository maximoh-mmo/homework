// Copyright 2025, Solar Corp. All right reserved


#include "Equip/EquipAreaWidget.h"

#include "GridItem.h"
#include "Components/NamedSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


void UEquipAreaWidget::RemoveItemCommand_Implementation(UGridItem* Item)
{
	DroppedItemSlot->ClearChildren();
	EquippedItem = nullptr;
	if (OnItemUnequipped.IsBound())
	{
		OnItemUnequipped.Broadcast(Item);
	}
}

bool UEquipAreaWidget::IsOccupied() const
{
	return IsValid(EquippedItem);
}

bool UEquipAreaWidget::DoesItemTagsMatch(UGridItem* InItem)
{
	if (!IsValid(InItem))
	{
		return false;
	}

	if (bMatchSlotTagsExact)
	{
		return InItem->ItemTags.HasAnyExact(SlotTags);
	}
	return InItem->ItemTags.HasAny(SlotTags);
}

bool UEquipAreaWidget::TryEquipItem_Implementation(UGridItem* InItem)
{
	ensureAlwaysMsgf(false, TEXT("TryEquipItem default implementation is called, you should override it in your class"));
	return false;
}

void UEquipAreaWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(TitleTextBlock))
	{
		TitleTextBlock->SetText(TitleText);
	}
	if (IsValid(TitleSizeBox))
	{
		// Title should not be bigger than area
		TitleSizeBox->SetMaxDesiredWidth(AreaWidth);
	}
	if (IsValid(AreaSizeBox))
	{
		AreaSizeBox->SetWidthOverride(AreaWidth);
		AreaSizeBox->SetHeightOverride(AreaHeight);
		AreaSizeBox->SetMaxDesiredWidth(AreaWidth);
		AreaSizeBox->SetMaxDesiredHeight(AreaHeight);
	}
}

void UEquipAreaWidget::EquipItemInternal(UGridItem* InItem)
{
	EquippedItem = InItem;
	EquippedItem->SetItemOwnerInterfaceObject(this);
	if (OnItemEquipped.IsBound())
	{
		OnItemEquipped.Broadcast(InItem);
	}
}
