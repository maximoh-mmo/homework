// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/GridWindowSpecification.h"
#include "Widgets/DraggableWindow/Interfaces/DraggableWindowContent.h"

#include "GridItemSplittingWidget.generated.h"

class UButton;
class USpinBox;
class UGridItem;
class UTextBlock;

DECLARE_DELEGATE(FOnItemSpllitingWidgetFinished);

UCLASS(Abstract)
class GRIDINVENTORY_API UGridItemSplittingWidget : public UUserWidget, public IDraggableWindowContent
{
	GENERATED_BODY()

public:
	FOnItemSpllitingWidgetFinished OnItemSplittingWidgetClosed;

	virtual void OnWindowClosed_Implementation() override;
	
	void SetupPopup(UGridItem* InSplittingItem, const FPopupItemSplittingCountDelegate& InConfirmCallback, const FPopupCallbackDelegate& InCancelCallback);
	void SetupPopupForTransferring(int32 InMaxCount, const FPopupItemSplittingCountDelegate& InConfirmCallback, const FPopupCallbackDelegate& InCancelCallback);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UButton> ConfirmButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<USpinBox> SplitSpinBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UTextBlock> MaxSplittingCountTextBlock = nullptr;

private:
	UFUNCTION()
	void OnConfirmButtonClicked();
	
	FPopupItemSplittingCountDelegate ConfirmCallback;
	FPopupCallbackDelegate CancelCallback;
};
