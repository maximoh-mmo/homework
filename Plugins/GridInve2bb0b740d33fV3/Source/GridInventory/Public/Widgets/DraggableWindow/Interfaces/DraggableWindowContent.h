// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DraggableWindowContent.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDraggableWindowContent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRIDINVENTORY_API IDraggableWindowContent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Inventory")
	void OnWindowClosed();

	virtual void OnWindowClosed_Implementation();
};
