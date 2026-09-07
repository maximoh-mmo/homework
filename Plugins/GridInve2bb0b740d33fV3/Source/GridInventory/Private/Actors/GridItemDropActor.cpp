// Copyright 2025, Solar Corp. All right reserved


#include "Actors/GridItemDropActor.h"

#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GridItem.h"

AGridItemDropActor::AGridItemDropActor()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootScene");
	SetRootComponent(RootSceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DropMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("DropSkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* AGridItemDropActor::GetDropStaticMeshComponent() const
{
	return StaticMeshComponent;
}

USkeletalMeshComponent* AGridItemDropActor::GetDropSkeletalMeshComponent() const
{
	return SkeletalMeshComponent;
}

UGridItem* AGridItemDropActor::GetItem() const
{
	return OwnerItem;
}

void AGridItemDropActor::SetMeshToDrop(USkeletalMesh* InSkeletalMesh, UStaticMesh* InStaticMesh)
{
	if (IsValid(InSkeletalMesh))
	{
		SkeletalMeshComponent->SetSkeletalMesh(InSkeletalMesh);
		SkeletalMeshComponent->SetSimulatePhysics(true);
	}
	if (IsValid(InStaticMesh))
	{
		StaticMeshComponent->SetStaticMesh(InStaticMesh);
		StaticMeshComponent->SetSimulatePhysics(true);
	}
}

void AGridItemDropActor::SetItem(UGridItem* InItem)
{
	if (!ensureAlwaysMsgf(IsValid(InItem), TEXT("Item is invalid")))
	{
		return;
	}

	InItem->SetIsRotated(false);
	
	OwnerItem = InItem;
	SetMeshToDrop(InItem->ItemSkeletalMesh.LoadSynchronous(), InItem->ItemMesh.LoadSynchronous());
}
