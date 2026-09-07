// Copyright 2025, Solar Corp. All right reserved

#include "Actors/GridItemPickupActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GridItem.h"
#include "InventoryAsset.h"
#include "InventoryComponent.h"


AGridItemPickupActor::AGridItemPickupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootScene");
	SetRootComponent(RootSceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DropMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("DropSkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

void AGridItemPickupActor::InitPickupActor()
{
	if (!ensureAlwaysMsgf(IsValid(InventoryAsset), TEXT("InventoryAsset is null, assign an asset for inventory")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(IsValid(DropItem), TEXT("DropItem is invalid, assign a valid item to DropItem")))
	{
		return;
	}
	
	Inventory = NewObject<UInventory>();
	Inventory->InitInventory(InventoryAsset);
	DropItem->InitItem();
	Inventory->TryAddItem(DropItem);
}
