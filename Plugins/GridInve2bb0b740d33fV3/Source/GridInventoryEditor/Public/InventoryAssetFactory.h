// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "InventoryAssetFactory.generated.h"


UCLASS()
class UInventoryAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UInventoryAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
