// Copyright 2025, Solar Corp. All right reserved


#include "Widgets/DraggableWindow/GridDraggableWindow.h"

#include "Components/NamedSlot.h"
#include "Components/OverlaySlot.h"
#include "Widgets/InventoryLayoutWidget.h"
#include "Widgets/DraggableWindow/Interfaces/DraggableWindowContent.h"


UObject* UGridDraggableWindow::GetWindowInstigator() const
{
	return WindowInstigator;
}

void UGridDraggableWindow::SetSpecification(const FGridWindowSpecification& InSpecification)
{
	OnSpecificationSet(InSpecification);
}

void UGridDraggableWindow::SetWindowInstigator(UObject* InInstigator)
{
	WindowInstigator = InInstigator;
}

void UGridDraggableWindow::SetContentPadding(const FMargin& InMargin)
{
	UOverlaySlot* contentAsOverlaySlot = Cast<UOverlaySlot>(ContentSlot->Slot);
	if (ensureAlwaysMsgf(IsValid(contentAsOverlaySlot), TEXT("ContentSlot is not inside overlay")))
	{
		contentAsOverlaySlot->SetPadding(InMargin);
	}
}

void UGridDraggableWindow::CloseWindow()
{
	OnWindowClosed();

	for (UWidget* contentWidget : ContentSlot->GetAllChildren())
	{
		if (contentWidget->Implements<UDraggableWindowContent>())
		{
			IDraggableWindowContent::Execute_OnWindowClosed(contentWidget);
		}
	}
	
	RemoveFromParent();

	if (OnDraggableWindowClosed.IsBound())
	{
		OnDraggableWindowClosed.Broadcast(this);
	}
}

void UGridDraggableWindow::BroadcastDragStarted()
{
	if (OnWindowDragStarted.IsBound())
	{
		OnWindowDragStarted.Broadcast(this);
	}
}

void UGridDraggableWindow::AddWidgetToContentSlot(UUserWidget* InWidget)
{
	ContentSlot->AddChild(InWidget);
}
