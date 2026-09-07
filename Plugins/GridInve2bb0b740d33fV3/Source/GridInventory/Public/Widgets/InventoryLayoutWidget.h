// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryLayoutWidget.generated.h"

class UOverlay;
class UInventoryWidget;
class UInventory;

UCLASS(Abstract)
class GRIDINVENTORY_API UInventoryLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void InitLayout(UInventory* InInventory);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UOverlay> RootOverlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
};
