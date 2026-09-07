// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridInventory/Public/Widgets/ItemContext/GridItemContextElement.h"

#include "GridItemContextWidget.generated.h"

class UVerticalBox;
class UGridItemActionBase;
class UGridItem;

/**
 * 
 */
UCLASS(Abstract)
class GRIDINVENTORY_API UGridItemContextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Grid Inventory")
	void SetupContext(UGridItem* InItem);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Grid Inventory", meta=(BindWidget))
	TObjectPtr<UVerticalBox> ElementsBox = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grid Inventory")
	TSubclassOf<UGridItemContextElement> ContextElementClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Grid Inventory")
	FMargin ElementPadding = FMargin(0.f, 5.f, 0.f, 0.f);

private:
	UFUNCTION()
	void OnActionElementClicked(UGridItemContextElement* InClickedElement);
	
	UPROPERTY(BlueprintReadOnly, Category="Grid Inventory", meta=(AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UGridItemActionBase>> Actions;

	UPROPERTY(BlueprintReadOnly, Category="Grid Inventory", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UGridItem> Item = nullptr;
};
