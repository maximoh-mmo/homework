// Copyright 2025, Solar Corp. All right reserved

#include "InventoryAssetAction.h"
#include "InventoryAsset.h"
#include "InventoryAssetEditorApp.h"

InventoryAssetAction::InventoryAssetAction(EAssetTypeCategories::Type Category)
	: mAssetCategory(Category)
{
}

FText InventoryAssetAction::GetName() const
{
	return FText::FromString(TEXT("Inventory Asset"));
}

FColor InventoryAssetAction::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* InventoryAssetAction::GetSupportedClass() const
{
	return UInventoryAsset::StaticClass();
}

void InventoryAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : InObjects)
	{
		UInventoryAsset* inventoryAsset = Cast<UInventoryAsset>(object);
		if (IsValid(inventoryAsset))
		{
			TSharedRef<InventoryAssetEditorApp> editor(new InventoryAssetEditorApp());
			editor->InitEditor(mode, EditWithinLevelEditor, inventoryAsset);
		}
	}
}

uint32 InventoryAssetAction::GetCategories()
{
	return mAssetCategory;
}
