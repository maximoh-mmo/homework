// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryUIBridgeInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYUIBRIDGERUNTIME_API UInventoryUIBridgeInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryUIBridgeInventoryComponent();

};
