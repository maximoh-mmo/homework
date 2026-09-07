// Copyright 2025, Solar Corp. All right reserved


#include "Widgets/GridItemWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Pawn.h"
#include "GridItem.h"
#include "InventoryAsset.h"
#include "Events/GridEvents.h"
#include "Events/GridGameplayMessageSubsystem.h"


bool UGridItemWidget::GetIsWidgetRotated() const
{
	return bIsRotated;
}

float UGridItemWidget::GetWidgetWidth() const
{
	return WidgetWidth;
}

float UGridItemWidget::GetWidgetHeight() const
{
	return WidgetHeight;
}

UGridItem* UGridItemWidget::GetItem() const
{
	return Item;
}

void UGridItemWidget::SetWidgetSize(float InWidth, float InHeight)
{
	WidgetWidth = InWidth;
	WidgetHeight = InHeight;
}

void UGridItemWidget::SetupItem(UGridItem* InItem, const UInventoryAsset* InInventoryAsset)
{
	const FIntPoint itemDimensions = InItem->GetDimensions();
	
	Item = InItem;
	Item->OnItemIconGenerated.AddUniqueDynamic(this, &UGridItemWidget::OnItemIconGenerated);
	Item->OnItemWidgetUpdateRequested.AddUniqueDynamic(this, &UGridItemWidget::UpdateItemDisplay);
	SetWidgetSize(itemDimensions.X * InInventoryAsset->InventoryDefinition.GridCellSize, itemDimensions.Y * InInventoryAsset->InventoryDefinition.GridCellSize);
	
	SetWidgetParams(InItem, WidgetWidth, WidgetHeight);
}

void UGridItemWidget::SetupItemForEquipSlot(UGridItem* InItem, float InWidth, float InHeight)
{
	Item = InItem;
	Item->OnItemIconGenerated.AddUniqueDynamic(this, &UGridItemWidget::OnItemIconGenerated);
	SetWidgetParams(InItem, InWidth, InHeight);
}

void UGridItemWidget::SetupWidgetForDragOperation(const UGridItem* const InItem, float InWidth, float InHeight)
{
	bIsRotated = InItem->GetIsRotated();
	SetWidgetParams(InItem, InWidth, InHeight);
}

void UGridItemWidget::ToggleRotateForDragOperation()
{
	bIsRotated = !bIsRotated;
	
	float height = ItemSizeBox->GetHeightOverride();
	float width = ItemSizeBox->GetWidthOverride();
	
	ItemSizeBox->SetWidthOverride(height);
	ItemSizeBox->SetHeightOverride(width);
	
	SetWidgetSize(height, width);

	UMaterialInstanceDynamic* dynamicMaterial = ItemImage->GetDynamicMaterial();
	dynamicMaterial->SetScalarParameterValue(FName("RotationAngle"), bIsRotated ? -0.25f : 0.0f);

	SetCorrectImageSize();
}

void UGridItemWidget::UpdateItemDisplay()
{
	if (!IsValid(Item))
	{
		return;
	}

	SetWidgetParams(Item, WidgetWidth, WidgetHeight);
}

void UGridItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitialBackgroundColor = BackgroundBorder->GetBrushColor();

	DragStartedListener = UGridGameplayMessageSubsystem::Get(this).RegisterListener<FGridItemDragEventData>(DragStartedEventTag, this, &UGridItemWidget::OnItemDragStarted);
	DragSplitStartedListener = UGridGameplayMessageSubsystem::Get(this).RegisterListener<FGridItemDragEventData>(DragSplitStartedEventTag, this, &UGridItemWidget::OnItemDragStarted);
	DragEndedListener = UGridGameplayMessageSubsystem::Get(this).RegisterListener<FGridItemDragEventData>(DragEndedEventTag, this, &UGridItemWidget::OnItemDragEnded);
	DragSplitEndedListener = UGridGameplayMessageSubsystem::Get(this).RegisterListener<FGridItemDragEventData>(DragSplitEndedEventTag, this, &UGridItemWidget::OnItemDragEnded);
}

void UGridItemWidget::NativeDestruct()
{
	DragStartedListener.Unregister();
	DragSplitStartedListener.Unregister();
	DragEndedListener.Unregister();
	DragSplitEndedListener.Unregister();
	
	Super::NativeDestruct();
}

void UGridItemWidget::BroadcastDragDropCreated(UGridPayload* InPayload)
{
	if (OnDragDropCreated.IsBound())
	{
		OnDragDropCreated.Broadcast(InPayload);
	}
}

void UGridItemWidget::SetWidgetParams(const UGridItem* const InItem, float InWidth, float InHeight)
{
	UTextureRenderTarget2D* generatedRenderTarget = InItem->GetItemGeneratedIcon();
	if (InItem->bUseGeneratedIcons)
	{
		if (IsValid(generatedRenderTarget))
		{
			IconSize = FIntPoint(generatedRenderTarget->SizeX, generatedRenderTarget->SizeY);
		}
	}
	else
	{
		if (IsValid(InItem->Icon))
		{
			IconSize = FIntPoint(InItem->Icon->GetSizeX(), InItem->Icon->GetSizeY());
		}
	}
	bIsRotated = InItem->GetIsRotated();
	ItemImage->SetBrushFromMaterial(IconMaterial);
	
	ItemSizeBox->SetWidthOverride(InWidth);
	ItemSizeBox->SetHeightOverride(InHeight);

	UTexture* generatedTexture = InItem->GetItemGeneratedIcon();
	UMaterialInstanceDynamic* dynamicMaterial = ItemImage->GetDynamicMaterial();
	dynamicMaterial->SetTextureParameterValue(FName("Texture"), InItem->bUseGeneratedIcons ? generatedTexture : InItem->Icon);
	dynamicMaterial->SetScalarParameterValue(FName("RotationAngle"), InItem->GetIsRotated() ? -0.25f : 0.0f);
	dynamicMaterial->SetScalarParameterValue(FName("UseGeneratedIcon"), InItem->bUseGeneratedIcons ? 1.f : -1.f);

	SetCorrectImageSize();

	ItemStacksTextBlock->SetVisibility(InItem->IsStackable() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (InItem->IsStackable())
	{
		FString stackStr = FString::Format(TEXT("{0}"), {InItem->GetCurrentStacks()});
		ItemStacksTextBlock->SetText(FText::FromString(stackStr));
	}
}

void UGridItemWidget::SetCorrectImageSize() const
{
	FSlateBrush brush = ItemImage->GetBrush();
	brush.ImageSize.X = bIsRotated ? IconSize.Y : IconSize.X;
	brush.ImageSize.Y = bIsRotated ? IconSize.X : IconSize.Y;
	ItemImage->SetBrush(brush);
}

void UGridItemWidget::OnItemIconGenerated(UTextureRenderTarget2D* InGeneratedIcon)
{
	if (IsValid(InGeneratedIcon))
	{
		IconSize = FIntPoint(InGeneratedIcon->SizeX, InGeneratedIcon->SizeY);
	}

	UMaterialInstanceDynamic* dynamicMaterial = ItemImage->GetDynamicMaterial();
	dynamicMaterial->SetTextureParameterValue(FName("Texture"), InGeneratedIcon);
	SetCorrectImageSize();
}

void UGridItemWidget::OnItemDragStarted(FGameplayTag InEventTag, const FGridItemDragEventData& InEvent)
{
	if (GetOwningPlayerPawn() != InEvent.EventInstigator)
	{
		return;
	}
	
	if (!IsValid(Item) || Item == InEvent.DraggedItem)
	{
		return;
	}

	// We use the same callback for 2 events, because they have the same data, so we just have to filter them here.
	if (InEventTag == DragStartedEventTag)
	{
		// TODO: we use this color in lots of places, we should create some kind of manager where we'll take it.
		// TODO: usually I would make a data asset and put this asset in game instance, but this is a plugin and I don't wanna pack it with game instance dependencies
		if (Item->CanInteractWithItem(InEvent.DraggedItem))
		{
			BackgroundBorder->SetBrushColor(FLinearColor(1.f, 1.f, 0.f, 0.5f));
		}
	}
	else if (InEventTag == DragSplitStartedEventTag)
	{
		if (Item->CanStackWith(InEvent.DraggedItem) && Item->IsStackable() && Item->GetCurrentStacks() < Item->GetMaxStacks())
		{
			BackgroundBorder->SetBrushColor(FLinearColor(1.f, 1.f, 0.f, 0.5f));
		}
	}
}

void UGridItemWidget::OnItemDragEnded(FGameplayTag InEventTag, const FGridItemDragEventData& InEvent)
{
	if (GetOwningPlayerPawn() != InEvent.EventInstigator)
	{
		return;
	}
	
	BackgroundBorder->SetBrushColor(InitialBackgroundColor);
}

