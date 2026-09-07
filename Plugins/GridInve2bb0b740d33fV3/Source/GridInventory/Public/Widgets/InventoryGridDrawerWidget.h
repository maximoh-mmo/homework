// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "InventoryAsset.h"
#include "Blueprint/UserWidget.h"

#include "InventoryGridDrawerWidget.generated.h"

class UInventoryAsset;

UCLASS()
class GRIDINVENTORY_API UInventoryGridDrawerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGridDefinition(const UInventoryAsset* InAsset, const FIntPoint& InGridDimensions);

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
private:
	FInventoryDefinition InventoryDefinition;
	FIntPoint GridDimensions = FIntPoint(15, 10);
};
