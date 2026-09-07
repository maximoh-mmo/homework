// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/GridItemOwner.h"
#include "UObject/Object.h"

#include "Inventory.generated.h"

class UGridItem;
class UInventoryWidget;
class UInventoryAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAdded, UGridItem*, Item, const FIntPoint&, TopLeftTile, int32, InventorySlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UGridItem*, Item);

USTRUCT()
struct GRIDINVENTORY_API FInventoryGridCell
{
	GENERATED_BODY()

	void Reset();
	bool IsEmpty() const;
	
	// Grid item have its size, so each grid cell that is occupied by this item, has reference to an item, that is why it is associated
	UPROPERTY()
	TObjectPtr<UGridItem> AssociatedItem = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct GRIDINVENTORY_API FInventorySlot
{
	GENERATED_BODY()

	bool CanFitItem(const FIntPoint& ItemDimension) const;
	const TArray<TPair<UGridItem*, FIntPoint>>& GetItems() const;
	
	UGridItem* GetItemInCell(const FIntPoint& InCellPos);

	void InitSlot();

	bool CanFitItem(UGridItem* InItem, FIntPoint& TopLeftTilePosOut, FIntPoint StartingPos = FIntPoint(-1, -1), bool StopIfFirstTileOccupied = false, bool IgnoreSelfItem = false);
	bool TryAddItem(UGridItem* InItem, FIntPoint& TopLeftTilePosOut);
	bool RemoveItem(UGridItem* InItem);
	
	// This function assumes that an Item could be placed on this top left tile pos
	void AddItem(UGridItem* InItem, const FIntPoint& TopLeftTilePos);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	FIntPoint SlotDimension = FIntPoint(0, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	FVector2D PositionOffset = FVector2D(0.f, 0.f);
	
private:
	FInventoryGridCell* GetGridCellInternal(int32 X, int32 Y);
	
	// This array represents a 2D structure, so we should access it like this: yIndex > SlotDimension.x + xIndex
	UPROPERTY()
	TArray<FInventoryGridCell> SlotGridCells;
	
	TArray<TPair<UGridItem*, FIntPoint>> ItemToTopLeftTileList;
};

UCLASS(Blueprintable, BlueprintType)
class GRIDINVENTORY_API UInventory : public UObject, public IGridItemOwner
{
	GENERATED_BODY()

public:
	virtual void RemoveItemCommand_Implementation(UGridItem* Item) override;
	virtual TArray<TSubclassOf<UGridItemActionBase>> GetOwnerActionClasses_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void InitInventory(UInventoryAsset* InAsset);
	
	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnItemRemoved OnItemRemoved;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	const UInventoryAsset* GetInventoryAsset() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	TArray<UGridItem*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool TryAddItem(UGridItem* InItem);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool CanAddItem(UGridItem* InItem);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridItem* TryGetItemFromTopLeftPos(int32 InSlotIndex, const FIntPoint& InTopLeftTilePos);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridItem* TryGetItemFromPos(int32 InSlotIndex, const FIntPoint& InTilePos);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool CanFitItem(UGridItem* InItem, int32 InSlotIndex, FIntPoint InStartingTile, bool StopIfFirstTileOccupied, bool IgnoreSelfItem, FIntPoint& TopLeftTilePosOut);

	// This method assumes that an item could be placed in the inventory starting from InDestinationTopLeftTile, you should call it only if CanFitItem returns true
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddItemInSlot(UGridItem* InItem, int32 InDestinationSlotIndex, FIntPoint InDestinationTopLeftTile);

	// This method assumes that an item could be placed in the inventory starting from InDestinationTopLeftTile, you should call it only if CanFitItem returns true
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SplitItemToSlot(UGridItem* InItem, int32 InDestinationSlotIndex, FIntPoint InDestinationTopLeftTile, int32 InStacks = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void StackItems(UGridItem* InStacksReceiver, UGridItem* InStackGiver, bool& OutWasFullTransfer);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void TransferItemsStack(UGridItem* InStacksReceiver, UGridItem* InStackGiver, int32 InStackCount, bool& OutWasFullTransfer);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void RemoveItemFromSlot(UGridItem* InItem, int32 InSourceSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void RemoveItem(UGridItem* InItem);

	const TArray<FInventorySlot>& GetInventorySlots() const;
	
private:
	static void StackItemsInternal(UGridItem* InStacksReceiver, UGridItem* InStackGiver, int32 InStackCout, bool& OutWasFullTransfer);

	UPROPERTY()
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY()
	TObjectPtr<UInventoryAsset> InventoryAsset = nullptr;
};
