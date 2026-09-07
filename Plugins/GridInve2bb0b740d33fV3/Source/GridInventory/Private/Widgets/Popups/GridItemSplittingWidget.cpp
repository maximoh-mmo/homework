// Copyright 2025, Solar Corp. All right reserved


#include "Widgets/Popups/GridItemSplittingWidget.h"

#include "GridItem.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"

void UGridItemSplittingWidget::SetupPopup(UGridItem* InSplittingItem, const FPopupItemSplittingCountDelegate& InConfirmCallback, const FPopupCallbackDelegate& InCancelCallback)
{
	ConfirmCallback = InConfirmCallback;
	CancelCallback = InCancelCallback;
	
	ConfirmButton->OnClicked.AddUniqueDynamic(this, &UGridItemSplittingWidget::OnConfirmButtonClicked);

	SplitSpinBox->SetValue(1);
	SplitSpinBox->SetMinValue(1);
	SplitSpinBox->SetMinSliderValue(1);
	SplitSpinBox->SetMaxValue(InSplittingItem->GetCurrentStacks());
	SplitSpinBox->SetMaxSliderValue(InSplittingItem->GetCurrentStacks());

	MaxSplittingCountTextBlock->SetText(FText::AsNumber(InSplittingItem->GetCurrentStacks()));
}

void UGridItemSplittingWidget::SetupPopupForTransferring(int32 InMaxCount, const FPopupItemSplittingCountDelegate& InConfirmCallback, const FPopupCallbackDelegate& InCancelCallback)
{
	ConfirmCallback = InConfirmCallback;
	CancelCallback = InCancelCallback;
	
	ConfirmButton->OnClicked.AddUniqueDynamic(this, &UGridItemSplittingWidget::OnConfirmButtonClicked);

	SplitSpinBox->SetValue(1);
	SplitSpinBox->SetMinValue(1);
	SplitSpinBox->SetMinSliderValue(1);
	SplitSpinBox->SetMaxValue(InMaxCount);
	SplitSpinBox->SetMaxSliderValue(InMaxCount);

	MaxSplittingCountTextBlock->SetText(FText::AsNumber(InMaxCount));
}

void UGridItemSplittingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SplitSpinBox->SetFocus();
}

void UGridItemSplittingWidget::OnConfirmButtonClicked()
{
	if (ConfirmCallback.IsBound())
	{
		ConfirmCallback.Execute(SplitSpinBox->GetValue());
	}

	if (OnItemSplittingWidgetClosed.IsBound())
	{
		OnItemSplittingWidgetClosed.Execute();
	}
}

void UGridItemSplittingWidget::OnWindowClosed_Implementation()
{
	if (CancelCallback.IsBound())
	{
		CancelCallback.Execute();
	}
}
