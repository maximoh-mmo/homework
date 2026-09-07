// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerInventoryWidget.generated.h"

class UNamedSlot;
class UInventoryLayoutWidget;
class UPlayerInventoryEquipWidget;

/**
 * 
 */
UCLASS()
class GRIDINVENTORY_API UPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UPlayerInventoryEquipWidget> PlayerInventoryEquip = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	TSubclassOf<UInventoryLayoutWidget> InventoryLayoutClass;
};
