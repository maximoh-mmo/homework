// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"

#include "InventoryComponent.generated.h"

class UGridItem;
class UInventory;
class UInventoryAsset;

UCLASS(meta=(BlueprintSpawnableComponent))
class GRIDINVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UInventory* GetInventory() const;

	UFUNCTION(BLueprintCallable, Category = "Grid Inventory")
	void InitInventory();
	
protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Grid Inventory")
	TArray<UGridItem*> InitialItems;

	UPROPERTY(EditAnywhere, Category = "Grid Inventory")
	TObjectPtr<UInventoryAsset> InventoryAsset = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UInventory> Inventory = nullptr;
};
