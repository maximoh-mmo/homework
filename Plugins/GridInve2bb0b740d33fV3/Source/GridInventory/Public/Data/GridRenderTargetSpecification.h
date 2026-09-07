// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"

#include "GridRenderTargetSpecification.generated.h"

/**
 * This struct allows you to specify offsets and params for default params for GridRenderTargetActor
 */
USTRUCT(Blueprintable, BlueprintType)
struct GRIDINVENTORY_API FGridRenderTargetSpecification
{
	GENERATED_BODY()

	// If zero, size will be calculated by this formula: Item's dimensions * Item Grid cell size
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Mesh Settings", meta=(ExposeOnSpawn))
	FIntPoint RenderTargetSizeOverride = FIntPoint(0, 0);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Mesh Settings", meta=(ExposeOnSpawn))
	float RenderTargetSizeScale = 2.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Mesh Settings", meta=(ExposeOnSpawn))
	FVector MeshPositionOffset = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Mesh Settings", meta=(ExposeOnSpawn))
	FRotator MeshRelativeRotationOffset = FRotator::ZeroRotator;

	// This is used during icons generation
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Camera Settings", meta=(ExposeOnSpawn))
	float TargetArmLengthOffset = 0.f;

	// This is used during items inspection
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Camera Settings", meta=(ExposeOnSpawn))
	float InspectionTargetArmLengthOffset = 0.f;
};
