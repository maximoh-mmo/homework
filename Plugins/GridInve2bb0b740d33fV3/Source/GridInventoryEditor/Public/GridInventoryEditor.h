// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGridInventoryEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace GridInventory
{
	inline const FName InventoryAssetAppMode = FName("InventoryAssetAppMode");

	inline const FName InventoryAssetPrimaryTabName = FName(TEXT("InventoryAssetPrimaryTab"));
	inline const FName InventoryAssetPropertiesTabName = FName(TEXT("InventoryAssetPropertiesTab"));
}