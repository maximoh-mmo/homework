// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GridInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGridInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Simple interface class for demonstration Grid Inventory capacities
 */
class GRIDINVENTORY_API IGridInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Interaction")
	bool TryInteract(AActor* InstigatorActor);

	virtual bool TryInteract_Implementation(AActor* InstigatorActor);
};
