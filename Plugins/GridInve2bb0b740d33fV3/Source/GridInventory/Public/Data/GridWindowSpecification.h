// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GridWindowSpecification.generated.h"

DECLARE_DYNAMIC_DELEGATE(FPopupCallbackDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopupItemSplittingCountDelegate, int32, Count);

USTRUCT(Blueprintable, BlueprintType)
struct GRIDINVENTORY_API FGridWindowSpecification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FText ToolbarText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FMargin ContentPadding = FMargin(10.f);
};
