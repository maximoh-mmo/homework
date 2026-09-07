// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/GridWindowSpecification.h"

#include "GridDraggableWindow.generated.h"

class UNamedSlot;
class UCanvasPanelSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWindowDragStarted, UGridDraggableWindow*, DraggableWindow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDraggableWindowClosed, UGridDraggableWindow*, DraggableWindow);

/**
 * 
 */
UCLASS(Abstract)
class GRIDINVENTORY_API UGridDraggableWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnWindowDragStarted OnWindowDragStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnDraggableWindowClosed OnDraggableWindowClosed;
	
	UObject* GetWindowInstigator() const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void CloseWindow();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void BroadcastDragStarted();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grid Inventory")
	void FocusWindow();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Grid Inventory")
	void UnfocusWindow();
	
	void SetSpecification(const FGridWindowSpecification& InSpecification);
	void SetWindowInstigator(UObject* InInstigator);
	void SetContentPadding(const FMargin& InMargin);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddWidgetToContentSlot(UUserWidget* InWidget);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Grid Inventory")
	void OnSpecificationSet(const FGridWindowSpecification& InSpecification);

	UFUNCTION(BlueprintImplementableEvent, Category = "Grid Inventory")
	void OnWindowClosed();
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory", meta=(BindWidget))
	TObjectPtr<UNamedSlot> ContentSlot = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UObject> WindowInstigator = nullptr;
};
