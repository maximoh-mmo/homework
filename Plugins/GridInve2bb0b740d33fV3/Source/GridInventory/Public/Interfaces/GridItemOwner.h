// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridInventory/Public/Actions/GridItemActionBase.h"

#include "GridItemOwner.generated.h"

class UGridItem;

// This class does not need to be modified.
UINTERFACE()
class UGridItemOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * An item can be owned by multiple entities, such as inventory, or equip slot
 * This interface is design for communication between them, so we can perform some actions on items,
 * without strongly referencing any particular owner.
 */
class GRIDINVENTORY_API IGridItemOwner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Inventory")
	TArray<TSubclassOf<UGridItemActionBase>> GetOwnerActionClasses() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Inventory")
	void RemoveItemCommand(UGridItem* Item);

	// Default implementations
	virtual TArray<TSubclassOf<UGridItemActionBase>> GetOwnerActionClasses_Implementation() const;
};
