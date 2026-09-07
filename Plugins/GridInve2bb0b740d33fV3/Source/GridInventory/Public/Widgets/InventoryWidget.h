// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/GridWindowSpecification.h"

#include "InventoryWidget.generated.h"

class USizeBox;
class UCanvasPanel;
class UGridItem;
class UGridItemWidget;
class UInventoryGridDrawerWidget;
class UInventory;
class UGridPayload;

UCLASS()
class GRIDINVENTORY_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void InitInventoryWidget(UInventory* InOwnerInventory, int32 InInventorySlotIndex);

	void SetSize(float InWidth, float InHeight);

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	FVector2D ClampDraggedItemSize(const FVector2D& InDraggedItemSize, const FIntPoint& InDraggedItemTopLeftTile, UGridItem* InDraggedItem) const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void StartItemSplitting(UGridPayload* InGridPayload, const FGridWindowSpecification& InWindowSpec, const FIntPoint& InDraggedItemTopLeftTile);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void StartItemTransferring(UGridItem* InStacksReceiver, UGridPayload* InGridPayload, const FGridWindowSpecification& InWindowSpec);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<USizeBox> RootSizeBox = nullptr;
	
	// Inventory canvas is used as a holder container for all items widget.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UCanvasPanel> InventoryCanvas = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UInventoryGridDrawerWidget> InventoryDrawerWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	TSubclassOf<UGridItemWidget> GridItemWidgetClass;

private:
	UFUNCTION()
	void OnItemCreatedPayload(UGridPayload* InPayload);

	UFUNCTION()
	void AddItem(UGridItem* InItem, const FIntPoint& InTopLeftPoint, int32 InSlotIndex);

	UFUNCTION()
	void OnItemRemoved(UGridItem* InItem);

	UFUNCTION()
	void OnItemSplittingCountSelected(int32 InSplittingCount);

	UFUNCTION()
	void OnItemTransferringCountSelected(int32 InTransferringCount);

	UFUNCTION()
	void OnItemSplittingCanceled();

	UFUNCTION()
	void OnItemTransferringCanceled();
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess), Category = "Grid Inventory")
	TObjectPtr<UInventory> OwnerInventory = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess), Category = "Grid Inventory")
	int32 InventorySlotIndex = 0;

	UPROPERTY()
	TArray<UGridItemWidget*> ItemWidgets;

	UPROPERTY()
	TObjectPtr<UGridPayload> SplittingItemPayload = nullptr;
	// Position to where splitting was requested.
	FIntPoint SplittingItemTopLeftTile;

	UPROPERTY()
	TObjectPtr<UGridItem> StackReceiverItem = nullptr;
};
