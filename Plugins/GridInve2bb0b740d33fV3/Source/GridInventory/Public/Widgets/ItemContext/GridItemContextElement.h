// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GridItemContextElement.generated.h"

class UButton;
class UTextBlock;
class UGridItemContextElement;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElementClicked, UGridItemContextElement*, ContextElement);

/**
 * 
 */
UCLASS(Abstract)
class GRIDINVENTORY_API UGridItemContextElement : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Grid Inventory")
	FOnElementClicked OnElementClicked;

	UFUNCTION(BlueprintCallable, Category="Grid Inventory")
	void SetupElement(FText InActionName);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, Category="Grid Inventory", meta=(BindWidget))
	TObjectPtr<UButton> ContextButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="Grid Inventory", meta=(BindWidget))
	TObjectPtr<UTextBlock> ContextNameTextBlock = nullptr;
	
private:
	UFUNCTION()
	void OnButtonClicked();
};
