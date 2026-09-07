// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GridInteractionInterface.h"

#include "GridItemDropActor.generated.h"

class USceneComponent;
class UStaticMesh;
class USkeletalMesh;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UGridItem;

UCLASS()
class GRIDINVENTORY_API AGridItemDropActor : public AActor, public IGridInteractionInterface
{
	GENERATED_BODY()

public:
	AGridItemDropActor();
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UStaticMeshComponent* GetDropStaticMeshComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	USkeletalMeshComponent* GetDropSkeletalMeshComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridItem* GetItem() const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetMeshToDrop(USkeletalMesh* InSkeletalMesh = nullptr, UStaticMesh* InStaticMesh = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetItem(UGridItem* InItem);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USceneComponent> RootSceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UGridItem> OwnerItem = nullptr;
};
