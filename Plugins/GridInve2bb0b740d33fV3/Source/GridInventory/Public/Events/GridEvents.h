// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GridEvents.generated.h"

class UGridItem;

USTRUCT(Blueprintable, BlueprintType)
struct FGridItemDragEventData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	UObject* EventInstigator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	UGridItem* DraggedItem = nullptr;
};
