// Copyright 2025, Solar Corp. All right reserved

#include "GridInventoryEditor.h"
#include "InventoryAssetAction.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FGridInventoryEditorModule"

void FGridInventoryEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& assetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName("InventoryAsset"), FText::FromString("Inventory Asset"));
	TSharedPtr<InventoryAssetAction> inventoryAssetAction = MakeShareable(new InventoryAssetAction(assetType));
	assetToolsModule.RegisterAssetTypeActions(inventoryAssetAction.ToSharedRef());
}

void FGridInventoryEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGridInventoryEditorModule, GridInventoryEditor)