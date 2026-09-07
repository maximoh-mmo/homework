// Copyright 2025, Solar Corp. All right reserved

#include "Widgets/InventoryLayoutWidget.h"

#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "InventoryAsset.h"
#include "GridInventory/Public/Widgets/InventoryWidget.h"


void UInventoryLayoutWidget::InitLayout(UInventory* InInventory)
{
	RootOverlay->ClearChildren();

	for (int32 i = 0; i < InInventory->GetInventoryAsset()->InventorySlots.Num(); ++i)
	{
		UInventorySlotWrapper* inventoryWrapper = InInventory->GetInventoryAsset()->InventorySlots[i];
		
		const FVector2D& inventoryPosOffset = inventoryWrapper->InventorySlot.PositionOffset;
		UInventoryWidget* inventoryWidget = CreateWidget<UInventoryWidget>(GetOwningPlayer(), InventoryWidgetClass);
		inventoryWidget->InitInventoryWidget(InInventory, i);
		
		UOverlaySlot* inventoryAsSlot = RootOverlay->AddChildToOverlay(inventoryWidget);
		inventoryAsSlot->SetPadding(FMargin(inventoryPosOffset.X, inventoryPosOffset.Y, 0.f, 0.f));
	}
}
