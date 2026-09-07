// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SGridDrawer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGridDrawer)
		: _OnMouseButtonDown()
		, _GridSize(50.f)
		, _LineThickness(2.f)
		, _GridDimensions(FIntPoint(0, 0))
		, _IsSelected(false)
		, _GridColor(FLinearColor::White)
		{ }

	SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)
		
	SLATE_ATTRIBUTE(float, GridSize)
	SLATE_ATTRIBUTE(float, LineThickness)
	SLATE_ATTRIBUTE(FIntPoint, GridDimensions)
	SLATE_ATTRIBUTE(bool, IsSelected)
	SLATE_ATTRIBUTE(FLinearColor, GridColor)
		
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
	float mGridSize = 0.f;
	float mLineThickness = 0.f;
	FIntPoint mGridDimensions = FIntPoint(0, 0);
	bool bIsSelected = false;
	FLinearColor mGridColor = FLinearColor::White;
};
