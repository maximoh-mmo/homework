// Copyright 2025, Solar Corp. All right reserved


#include "Widgets/DraggableWindow/GridWindowsLayoutWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Widgets/InventoryLayoutWidget.h"
#include "Widgets/DraggableWindow/GridDraggableWindow.h"
#include "Widgets/ItemInspect/GridItemInspectWidget.h"
#include "GridItem.h"
#include "Inventory.h"
#include "Components/Image.h"
#include "Widgets/Popups/GridItemSplittingWidget.h"


void UGridWindowsLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ActiveWindows.Reserve(MaxWindowsCount);
}

UGridDraggableWindow* UGridWindowsLayoutWidget::ShowInventoryWindow(const FGridWindowSpecification& InWindowSpecification, UInventory* InInventory)
{
	if (!ensureAlwaysMsgf(IsValid(InventoryLayoutClass), TEXT("InventoryLayoutClass is null, please assign a valid class")))
	{
		return nullptr;
	}

	UInventoryLayoutWidget* inventoryLayout = CreateWidget<UInventoryLayoutWidget>(GetOwningPlayer(), InventoryLayoutClass);
	inventoryLayout->InitLayout(InInventory);
	
	UGridDraggableWindow* window = CreateWindow(InInventory, InWindowSpecification, inventoryLayout);
	return window;
}

UGridDraggableWindow* UGridWindowsLayoutWidget::ShowInspectWindow(const FGridWindowSpecification& InWindowSpecification,
	UGridItem* InInspectedItem)
{
	if (!ensureAlwaysMsgf(IsValid(ItemInspectWidgetClass), TEXT("ItemInspectWidgetClass is null, please assign a valid class")))
	{
		return nullptr;
	}

	UGridItemInspectWidget* itemInspectWidget = CreateWidget<UGridItemInspectWidget>(GetOwningPlayer(), ItemInspectWidgetClass);
	itemInspectWidget->SetInspectedItem(InInspectedItem);
	
	UGridDraggableWindow* window = CreateWindow(InInspectedItem, InWindowSpecification, itemInspectWidget);
	return window;
}

UGridDraggableWindow* UGridWindowsLayoutWidget::ShowSplitItemPopupWindow(const FGridWindowSpecification& InWindowSpecification,
	UGridItem* InSplittingItem, FPopupItemSplittingCountDelegate ConfirmCallback, FPopupCallbackDelegate CancelCallback)
{
	if (!ensureAlwaysMsgf(IsValid(ItemSplittingWidgetClass), TEXT("ItemSplittingWidgetClass is null, please assign a valid class")))
	{
		return nullptr;
	}

	UGridItemSplittingWidget* itemSplittingWidget = CreateWidget<UGridItemSplittingWidget>(GetOwningPlayer(), ItemSplittingWidgetClass);
	itemSplittingWidget->SetupPopup(InSplittingItem, ConfirmCallback, CancelCallback);
	itemSplittingWidget->OnItemSplittingWidgetClosed.BindUObject(this, &UGridWindowsLayoutWidget::CloseCurrentPopupWindow);
	
	UGridDraggableWindow* window = CreatePopupWindow(InWindowSpecification, itemSplittingWidget);
	PopupWindowImage->SetVisibility(ESlateVisibility::Visible);
	
	return window;
}

UGridDraggableWindow* UGridWindowsLayoutWidget::ShowTransferringItemPopupWindow(const FGridWindowSpecification& InWindowSpecification,
	int32 InMaxCount, FPopupItemSplittingCountDelegate ConfirmCallback, FPopupCallbackDelegate CancelCallback)
{
	if (!ensureAlwaysMsgf(IsValid(ItemSplittingWidgetClass), TEXT("ItemSplittingWidgetClass is null, please assign a valid class")))
	{
		return nullptr;
	}

	UGridItemSplittingWidget* itemSplittingWidget = CreateWidget<UGridItemSplittingWidget>(GetOwningPlayer(), ItemSplittingWidgetClass);
	itemSplittingWidget->SetupPopupForTransferring(InMaxCount, ConfirmCallback, CancelCallback);
	itemSplittingWidget->OnItemSplittingWidgetClosed.BindUObject(this, &UGridWindowsLayoutWidget::CloseCurrentPopupWindow);
	
	UGridDraggableWindow* window = CreatePopupWindow(InWindowSpecification, itemSplittingWidget);
	PopupWindowImage->SetVisibility(ESlateVisibility::Visible);
	
	return window;
}

UGridDraggableWindow* UGridWindowsLayoutWidget::CreateWindow(UObject* InWindowInstigator, const FGridWindowSpecification& InWindowSpecification,
	UUserWidget* InContentWidget)
{
	if (!ensureAlwaysMsgf(IsValid(WindowClass), TEXT("WindowClass is null, please assign a valid class")))
	{
		return nullptr;
	}

	UnfocusAllWindows();
	
	for (UGridDraggableWindow* window : ActiveWindows)
	{
		if (window->GetWindowInstigator() == InWindowInstigator)
		{
			window->FocusWindow();
			return window;
		}
	}
	
	if (ActiveWindows.Num() == MaxWindowsCount)
	{
		UGridDraggableWindow* frontWindow = ActiveWindows[0];
		frontWindow->CloseWindow();
	}
	
	const int32 childrenCount = RootCanvasPanel->GetChildrenCount();
	
	FVector2D newWindowPosition = FVector2D::ZeroVector;
	
	// Check if we already added a widget before
	if (childrenCount > 0)
	{
		UWidget* lastWidget = RootCanvasPanel->GetChildAt(childrenCount - 1);
		UCanvasPanelSlot* lastWidgetAsCanvasSlot = Cast<UCanvasPanelSlot>(lastWidget->Slot);
		if (IsValid(lastWidgetAsCanvasSlot))
		{
			FVector2D lastWidgetSize = lastWidget->GetDesiredSize();

			// anchors with 0.5 alignment are needed for centering, since GetCachedGeometry isn't ready yet.
			if (!lastWidgetAsCanvasSlot->GetAlignment().IsNearlyZero())
			{
				newWindowPosition = lastWidgetAsCanvasSlot->GetPosition() + NewWindowPositionOffset - FVector2D(lastWidgetSize.X / 2.f, lastWidgetSize.Y / 2.f);
			}
			else
			{
				newWindowPosition = lastWidgetAsCanvasSlot->GetPosition() + NewWindowPositionOffset;
			}
		}
	}
	
	UGridDraggableWindow* window = CreateWidget<UGridDraggableWindow>(GetOwningPlayer(), WindowClass);
	window->OnWindowDragStarted.AddUniqueDynamic(this, &UGridWindowsLayoutWidget::OnWindowDragStarted);
	window->OnDraggableWindowClosed.AddUniqueDynamic(this, &UGridWindowsLayoutWidget::OnWindowClosed);
	window->SetWindowInstigator(InWindowInstigator);
	window->SetSpecification(InWindowSpecification);
	window->SetContentPadding(InWindowSpecification.ContentPadding);
	window->AddWidgetToContentSlot(InContentWidget);

	UCanvasPanelSlot* windowAsSlot = RootCanvasPanel->AddChildToCanvas(window);
	windowAsSlot->SetPosition(newWindowPosition);
	windowAsSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	windowAsSlot->SetAutoSize(true);
	
	// Center first window
	if (childrenCount == 0)
	{
		windowAsSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	}

	ActiveWindows.Add(window);
	return window;
}

UGridDraggableWindow* UGridWindowsLayoutWidget::CreatePopupWindow(const FGridWindowSpecification& InWindowSpecification,
	UUserWidget* InContentWidget)
{
	if (!ensureAlwaysMsgf(IsValid(WindowClass), TEXT("WindowClass is null, please assign a valid class")))
	{
		return nullptr;
	}

	UGridDraggableWindow* window = CreateWidget<UGridDraggableWindow>(GetOwningPlayer(), WindowClass);
	window->OnDraggableWindowClosed.AddUniqueDynamic(this, &UGridWindowsLayoutWidget::OnWindowClosed);
	window->SetSpecification(InWindowSpecification);
	window->SetContentPadding(InWindowSpecification.ContentPadding);
	window->AddWidgetToContentSlot(InContentWidget);

	UCanvasPanelSlot* windowAsSlot = WindowPopupCanvasPanel->AddChildToCanvas(window);
	windowAsSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	windowAsSlot->SetAutoSize(true);
	windowAsSlot->SetAlignment(FVector2D(0.5f, 0.5f));

	ensureAlwaysMsgf(!IsValid(CurrentPopupWindow), TEXT("CurrentPopupWindow is valid, only one popup window should be active"));
	
	CurrentPopupWindow = window;
	
	return window;
}

void UGridWindowsLayoutWidget::OnWindowDragStarted(UGridDraggableWindow* InClosedWindow)
{
	UnfocusAllWindows();
}

void UGridWindowsLayoutWidget::OnWindowClosed(UGridDraggableWindow* InClosedWindow)
{
	if (InClosedWindow == CurrentPopupWindow)
	{
		PopupWindowImage->SetVisibility(ESlateVisibility::Collapsed);
		CurrentPopupWindow = nullptr;
	}
	
	ActiveWindows.Remove(InClosedWindow);
}

void UGridWindowsLayoutWidget::ClearLayout()
{
	// we copy pointers to another array, because we need to clear ActiveWindows so we don't remove an item during iterations
	TArray<UGridDraggableWindow*> activeWindows = ActiveWindows;
	ActiveWindows.Empty();
	
	for (UGridDraggableWindow* window : activeWindows)
	{
		window->CloseWindow();
	}

	CloseCurrentPopupWindow();
}

void UGridWindowsLayoutWidget::UnfocusAllWindows()
{
	for (UGridDraggableWindow* window : ActiveWindows)
	{
		window->UnfocusWindow();
	}
}

void UGridWindowsLayoutWidget::CloseCurrentPopupWindow()
{
	if (IsValid(CurrentPopupWindow))
	{
		CurrentPopupWindow->CloseWindow();
		CurrentPopupWindow = nullptr;
		PopupWindowImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

