// Copyright 2025, Solar Corp. All right reserved


#include "GridInventory/Public/Interfaces/GridCharacterInterface.h"


UGridWindowsLayoutWidget* IGridCharacterInterface::GetGridWindowsLayoutWidget_Implementation() const
{
	ensureAlwaysMsgf(false, TEXT("GetGridWindowsLayoutWidget is not implemented, please implement it in your character"));
	return nullptr;
}

void IGridCharacterInterface::DropItem_implementation(UGridItem* InItem)
{
	ensureAlwaysMsgf(false, TEXT("DropItem is not implemented, please implement it in your character"));
}

FTransform IGridCharacterInterface::GetItemDropPointTransform_Implementation() const
{
	return FTransform::Identity;
}
