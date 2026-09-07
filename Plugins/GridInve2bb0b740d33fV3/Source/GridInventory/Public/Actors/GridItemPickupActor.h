// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GridInteractionInterface.h"

#include "GridItemPickupActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UInventoryComponent;
class UInventoryAsset;
class UInventory;
class UGridItem;

UCLASS()
class GRIDINVENTORY_API AGridItemPickupActor : public AActor, public IGridInteractionInterface
{
	GENERATED_BODY()

public:
	AGridItemPickupActor();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void InitPickupActor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USceneComponent> RootSceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Grid Inventory")
	TObjectPtr<UInventoryAsset> InventoryAsset = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Grid Inventory")
	TObjectPtr<UGridItem> DropItem = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UInventory> Inventory = nullptr;
};
