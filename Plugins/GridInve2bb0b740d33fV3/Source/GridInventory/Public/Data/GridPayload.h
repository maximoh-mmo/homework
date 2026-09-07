// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GridPayload.generated.h"

class UGridItem;
class UInventory;
class UGridItemWidget;

UCLASS(Blueprintable, BlueprintType)
class GRIDINVENTORY_API UGridPayload : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void ResetItemInitialRotation();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetItemRotation(bool InRotation);
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Category = "Grid Inventory")
	TObjectPtr<UGridItem> GridItem = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Category = "Grid Inventory")
	bool bIsItemInitiallyRotated = false;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Category = "Grid Inventory")
	TObjectPtr<UGridItemWidget> GridItemWidget = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category = "Grid Inventory")
	TObjectPtr<UInventory> SourceInventory = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Grid Inventory")
	int32 SourceInventorySlotIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Grid Inventory")
	bool bShouldTrySplit = false;
};
