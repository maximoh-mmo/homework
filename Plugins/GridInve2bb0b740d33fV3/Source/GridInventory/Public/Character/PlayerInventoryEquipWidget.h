// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerInventoryEquipWidget.generated.h"

class UNamedSlot;
class UInventory;
class UInventoryLayoutWidget;
class UGridItem;
class UEquipAreaWidget;

/**
 * 
 */
UCLASS(Abstract)
class GRIDINVENTORY_API UPlayerInventoryEquipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	const TArray<UInventory*>& GetEquippedInventories() const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddInventoryToPocketsSlot(UInventory* InInventory);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddInventoryToBackpackSlot(UInventory* InInventory);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddInventoryToRigSlot(UInventory* InInventory);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddLayoutToSlot(UInventory* InInventory, UNamedSlot* InInventorySlot);

	// This function will try to equip the item in a backpack or rig slot. 
	// If slots are occupied or the item cannot be placed in these slots,
	// it will try to put it inside equipped inventories if available.
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool TryEquipItemInAnyAvailableSlotOrInventory(UGridItem* InItem);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void RemoveEquippedInventory(UInventory* InEquippedInventory);
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UNamedSlot> PocketsInventorySlot = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UNamedSlot> BackpackInventorySlot = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UNamedSlot> RigInventorySlot = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), category = "Grid Inventory")
	TObjectPtr<UEquipAreaWidget> RigEquipArea = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), category = "Grid Inventory")
	TObjectPtr<UEquipAreaWidget> BackpackEquipArea = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	TSubclassOf<UInventoryLayoutWidget> InventoryLayoutClass;

private:
	UPROPERTY()
	TArray<TObjectPtr<UInventory>> EquippedInventories;
};
