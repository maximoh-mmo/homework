// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/GridWindowSpecification.h"

#include "GridWindowsLayoutWidget.generated.h"

class UImage;
class UCanvasPanel;
class UGridDraggableWindow;
class UGridItemInspectWidget;
class UInventoryLayoutWidget;
class UGridItemSplittingWidget;
class UInventory;
class UGridItem;

/**
 * 
 */
UCLASS()
class GRIDINVENTORY_API UGridWindowsLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridDraggableWindow* ShowInventoryWindow(UPARAM(ref) const FGridWindowSpecification& InWindowSpecification, UInventory* InInventory);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridDraggableWindow* ShowInspectWindow(UPARAM(ref) const FGridWindowSpecification& InWindowSpecification, UGridItem* InInspectedItem);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridDraggableWindow* ShowSplitItemPopupWindow(UPARAM(ref) const FGridWindowSpecification& InWindowSpecification, UGridItem* InSplittingItem, FPopupItemSplittingCountDelegate ConfirmCallback, FPopupCallbackDelegate CancelCallback);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridDraggableWindow* ShowTransferringItemPopupWindow(UPARAM(ref) const FGridWindowSpecification& InWindowSpecification, int32 InMaxCount, FPopupItemSplittingCountDelegate ConfirmCallback, FPopupCallbackDelegate CancelCallback);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void ClearLayout();

protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> RootCanvasPanel = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> WindowPopupCanvasPanel = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory", meta=(BindWidget))
	TObjectPtr<UImage> PopupWindowImage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TSubclassOf<UGridDraggableWindow> WindowClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TSubclassOf<UInventoryLayoutWidget> InventoryLayoutClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TSubclassOf<UGridItemInspectWidget> ItemInspectWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TSubclassOf<UGridItemSplittingWidget> ItemSplittingWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	FVector2D NewWindowPositionOffset = FVector2D(40.f, 40.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	int MaxWindowsCount = 4;

private:
	UGridDraggableWindow* CreateWindow(UObject* InWindowInstigator, const FGridWindowSpecification& InWindowSpecification, UUserWidget* InContentWidget);
	UGridDraggableWindow* CreatePopupWindow(const FGridWindowSpecification& InWindowSpecification, UUserWidget* InContentWidget);

	UFUNCTION()
	void OnWindowDragStarted(UGridDraggableWindow* InClosedWindow);
	UFUNCTION()
	void OnWindowClosed(UGridDraggableWindow* InClosedWindow);

	UFUNCTION()
	void CloseCurrentPopupWindow();

	void UnfocusAllWindows();

	UPROPERTY()
	TArray<TObjectPtr<UGridDraggableWindow>> ActiveWindows;

	UPROPERTY()
	TObjectPtr<UGridDraggableWindow> CurrentPopupWindow = nullptr;
};
