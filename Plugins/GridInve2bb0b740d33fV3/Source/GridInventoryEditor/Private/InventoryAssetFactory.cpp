// Copyright 2025, Solar Corp. All right reserved


#include "InventoryAssetFactory.h"

#include "InventoryAsset.h"

UInventoryAssetFactory::UInventoryAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UInventoryAsset::StaticClass();
}

UObject* UInventoryAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	UInventoryAsset* asset = NewObject<UInventoryAsset>(InParent, InName, Flags);
	return asset;
}

bool UInventoryAssetFactory::CanCreateNew() const
{
	return true;
}
