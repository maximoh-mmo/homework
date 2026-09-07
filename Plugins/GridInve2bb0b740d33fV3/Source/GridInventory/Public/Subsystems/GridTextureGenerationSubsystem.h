// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "GridTextureGenerationSubsystem.generated.h"

class AGridRenderTargetActor;

USTRUCT(Blueprintable, BlueprintType)
struct GRIDINVENTORY_API FGridRenderTargetSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn="true"))
	TSubclassOf<AGridRenderTargetActor> RenderActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn="true"))
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn="true"))
	FRotator SpawnRotation = FRotator::ZeroRotator;
};

/**
 * 
 */
UCLASS()
class GRIDINVENTORY_API UGridTextureGenerationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	AGridRenderTargetActor* GetAvailableRenderTargetActor();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	AGridRenderTargetActor* GetIconGenerationRenderActor();
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	TArray<AGridRenderTargetActor*> SpawnGridRenderActors(UPARAM(ref) const FGridRenderTargetSpawnParams& SpawnParams, int32 ActorsCount, FVector SpawnOffset);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	AGridRenderTargetActor* SpawnIconGenerationRenderActor(UPARAM(ref) const FGridRenderTargetSpawnParams& SpawnParams);

private:
	UPROPERTY()
	TArray<AGridRenderTargetActor*> SpawnedGridRenderActors;

	// This is a dedicated render target actor that is used for generation thumbnail icons for inventory items
	UPROPERTY()
	AGridRenderTargetActor* IconGenerationRenderActor = nullptr;
};
