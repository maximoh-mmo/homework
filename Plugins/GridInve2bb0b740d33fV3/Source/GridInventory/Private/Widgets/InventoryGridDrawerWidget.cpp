// Copyright 2025, Solar Corp. All right reserved

#include "Widgets/InventoryGridDrawerWidget.h"

#include "InventoryAsset.h"

void UInventoryGridDrawerWidget::SetGridDefinition(const UInventoryAsset* InAsset, const FIntPoint& InGridDimensions)
{
	InventoryDefinition = InAsset->InventoryDefinition;
	GridDimensions = InGridDimensions;
}

int32 UInventoryGridDrawerWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const float gridSize = InventoryDefinition.GridCellSize;
	const FLinearColor lineColor = InventoryDefinition.GridLineColor;
	const float lineThickness = InventoryDefinition.GridLineThickness;

	TArray<FVector2D> LinePoints;

	const int inventoryXSize =GridDimensions.X;
	const int inventoryYSize = GridDimensions.Y;

	// Draw vertical lines
	for (int x = 0; x <= inventoryXSize; ++x)
	{
		LinePoints.Reset();
		LinePoints.Add(FVector2D(x * gridSize, 0));
		LinePoints.Add(FVector2D(x * gridSize, inventoryYSize * gridSize));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			LinePoints,
			ESlateDrawEffect::None,
			lineColor,
			false,
			lineThickness
		);
	}

	// Draw horizontal lines
	for (int y = 0; y <= inventoryYSize; ++y)
	{
		LinePoints.Reset();
		LinePoints.Add(FVector2D(0, y * gridSize));
		LinePoints.Add(FVector2D(inventoryXSize * gridSize, y * gridSize));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			LinePoints,
			ESlateDrawEffect::None,
			lineColor,
			false,
			lineThickness
		);
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
