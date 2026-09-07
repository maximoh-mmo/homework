// Copyright 2025, Solar Corp. All right reserved

#include "SlateWidgets/GridDrawer.h"


void SGridDrawer::Construct(const FArguments& InArgs)
{
	if (InArgs._OnMouseButtonDown.IsBound())
	{
		SetOnMouseButtonDown(InArgs._OnMouseButtonDown);
	}
	
	mGridSize = InArgs._GridSize.Get();
	mLineThickness = InArgs._LineThickness.Get();
	mGridDimensions = InArgs._GridDimensions.Get();
	bIsSelected = InArgs._IsSelected.Get();
	mGridColor = InArgs._GridColor.Get();

	SetCanTick(true);
}

int32 SGridDrawer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	const FLinearColor lineColor = bIsSelected ? FLinearColor::Yellow : mGridColor;

	TArray<FVector2D> LinePoints;

	const int inventoryXSize = mGridDimensions.X;
	const int inventoryYSize = mGridDimensions.Y;
	
	// Draw vertical lines
	for (int x = 0; x <= inventoryXSize; ++x)
	{
		LinePoints.Reset();
		LinePoints.Add(FVector2D(x * mGridSize, 0));
		LinePoints.Add(FVector2D(x * mGridSize, inventoryYSize * mGridSize));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			LinePoints,
			ESlateDrawEffect::None,
			lineColor,
			false,
			mLineThickness
		);
	}

	// Draw horizontal lines
	for (int y = 0; y <= inventoryYSize; ++y)
	{
		LinePoints.Reset();
		LinePoints.Add(FVector2D(0, y * mGridSize));
		LinePoints.Add(FVector2D(inventoryXSize * mGridSize, y * mGridSize));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			LinePoints,
			ESlateDrawEffect::None,
			lineColor,
			false,
			mLineThickness
		);
	}
	
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
