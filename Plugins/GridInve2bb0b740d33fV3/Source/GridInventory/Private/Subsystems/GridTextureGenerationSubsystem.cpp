// Copyright 2025, Solar Corp. All right reserved


#include "Subsystems/GridTextureGenerationSubsystem.h"
#include "Engine/World.h"
#include "GridInventory/Public/Actors/GridRenderTargetActor.h"


AGridRenderTargetActor* UGridTextureGenerationSubsystem::GetAvailableRenderTargetActor()
{
	for (AGridRenderTargetActor* renderActor : SpawnedGridRenderActors)
	{
		if (!renderActor->GetIsOccupied())
		{
			return renderActor;
		}
	}

	ensureAlwaysMsgf(false, TEXT("No available render actors were found, perhaps you might want to spawn more"));
	return nullptr;
}

AGridRenderTargetActor* UGridTextureGenerationSubsystem::GetIconGenerationRenderActor()
{
	return IconGenerationRenderActor;
}

TArray<AGridRenderTargetActor*> UGridTextureGenerationSubsystem::SpawnGridRenderActors(const FGridRenderTargetSpawnParams& SpawnParams, int32 ActorsCount, FVector SpawnOffset)
{
	TArray<AGridRenderTargetActor*> spawnedActors;
	spawnedActors.Reserve(ActorsCount);
	
	for (int32 i = 0; i < ActorsCount; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		
		AGridRenderTargetActor* renderTargetActor = GetWorld()->SpawnActor<AGridRenderTargetActor>(SpawnParams.RenderActorClass, SpawnParams.SpawnLocation + i * SpawnOffset, SpawnParams.SpawnRotation, SpawnInfo);
		spawnedActors.Add(renderTargetActor);
	}

	SpawnedGridRenderActors.Append(spawnedActors);
	return spawnedActors;
}

AGridRenderTargetActor* UGridTextureGenerationSubsystem::SpawnIconGenerationRenderActor(const FGridRenderTargetSpawnParams& SpawnParams)
{
	FActorSpawnParameters SpawnInfo;
		
	IconGenerationRenderActor = GetWorld()->SpawnActor<AGridRenderTargetActor>(SpawnParams.RenderActorClass, SpawnParams.SpawnLocation, SpawnParams.SpawnRotation, SpawnInfo);
	if (ensureAlwaysMsgf(IsValid(IconGenerationRenderActor), TEXT("Error during IconGenerationRenderActor spawning")))
	{
		IconGenerationRenderActor->SetCaptureEveryFrame(false);
	}

	return IconGenerationRenderActor;
}
