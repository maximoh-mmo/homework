// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Interfaces/GridItemOwner.h"

#include "EquipAreaWidget.generated.h"

class USizeBox;
class UTextBlock;
class UNamedSlot;
class UGridItem;
class UGridItemWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, UGridItem*, EquippedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, UGridItem*, UnequippedItem);

/**
 * 
 */
UCLASS()
class GRIDINVENTORY_API UEquipAreaWidget : public UUserWidget, public IGridItemOwner
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnItemEquipped OnItemEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnItemUnequipped OnItemUnequipped;
	
	virtual void RemoveItemCommand_Implementation(UGridItem* Item) override;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool IsOccupied() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool DoesItemTagsMatch(UGridItem* InItem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Inventory")
	bool TryEquipItem(UGridItem* InItem);

	virtual bool TryEquipItem_Implementation(UGridItem* InItem);

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void EquipItemInternal(UGridItem* InItem);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UTextBlock> TitleTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<USizeBox> TitleSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<USizeBox> AreaSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UNamedSlot> DroppedItemSlot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FText TitleText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	float AreaWidth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	float AreaHeight = 100.f;

	// SlotTags determines what objects can be placed in this slot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FGameplayTagContainer SlotTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	bool bMatchSlotTagsExact = false;

private:
	UPROPERTY()
	TObjectPtr<UGridItem> EquippedItem = nullptr;
};
