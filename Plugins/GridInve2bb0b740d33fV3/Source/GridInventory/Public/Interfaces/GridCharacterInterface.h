// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GridCharacterInterface.generated.h"

class UGridWindowsLayoutWidget;
class UGridItem;

// This class does not need to be modified.
UINTERFACE()
class UGridCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface serves as a data provider for inventory elements from the character. We use this interface so that the client user 
 * can work with any character without directly depending on GridInventoryCharacter.
 * GridInventoryCharacter exists as a logic example for this plugin.
 */
class GRIDINVENTORY_API IGridCharacterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Character Interface")
	UGridWindowsLayoutWidget* GetGridWindowsLayoutWidget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Character Interface")
	void DropItem(UGridItem* InItem);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Character Interface")
	FTransform GetItemDropPointTransform();

	// Default implementations
	virtual UGridWindowsLayoutWidget* GetGridWindowsLayoutWidget_Implementation() const;
	virtual void DropItem_implementation(UGridItem* InItem);
	virtual FTransform GetItemDropPointTransform_Implementation() const;
};
