// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridInventory/Public/Actions/GridItemActionBase.h"
#include "Inventory.h"

#include "InventoryAsset.generated.h"


// a simple wrapper for FInventorySlot struct, so we can pass it to details view
UCLASS(EditInlineNew)
class UInventorySlotWrapper : public UObject
{
	GENERATED_BODY()

public:
	UInventorySlotWrapper() = default;
	
	UPROPERTY(EditAnywhere, Category = "Grid Inventory")
	FInventorySlot InventorySlot;
};

USTRUCT(Blueprintable, BlueprintType)
struct GRIDINVENTORY_API FInventoryDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	int32 GridCellSize = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	float GridLineThickness = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FLinearColor GridLineColor = FLinearColor::White;

	// Those actions will be provided to each item that is inside this inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	TArray<TSubclassOf<UGridItemActionBase>> ActionsToGrant;
};

DECLARE_MULTICAST_DELEGATE(FOnInventorySlotsModified)

UCLASS(BlueprintType)
class GRIDINVENTORY_API UInventoryAsset : public UObject
{
	GENERATED_BODY()

public:
	void AddNewSlot();
	void RemoveLastSlot();
	void EmptySlots();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FInventoryDefinition InventoryDefinition;

	UPROPERTY(EditAnywhere, Category = "Grid Inventory")
	float SpaceBetweenElements = 5.f;
	
	UPROPERTY(Instanced)
	TArray<UInventorySlotWrapper*> InventorySlots;
};
