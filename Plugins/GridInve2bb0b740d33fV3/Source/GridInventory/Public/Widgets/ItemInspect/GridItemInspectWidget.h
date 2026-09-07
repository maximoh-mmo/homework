// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/DraggableWindow/Interfaces/DraggableWindowContent.h"

#include "GridItemInspectWidget.generated.h"

class UGridItem;

/**
 * 
 */
UCLASS()
class GRIDINVENTORY_API UGridItemInspectWidget : public UUserWidget, public IDraggableWindowContent
{
	GENERATED_BODY()

public:
	void SetInspectedItem(UGridItem* InInspectedItem);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory")
	UGridItem* InspectedItem = nullptr;
};
