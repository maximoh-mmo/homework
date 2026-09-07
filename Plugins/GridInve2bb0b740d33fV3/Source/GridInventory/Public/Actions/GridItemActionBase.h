// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridInventory/Public/Widgets/ItemContext/GridItemContextElement.h"

#include "GridItemActionBase.generated.h"

class UGridItem;
class UGridItemContextWidget;

USTRUCT(Blueprintable, BlueprintType)
struct GRIDINVENTORY_API FGridActionContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	TObjectPtr<UGridItem> Item = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	TObjectPtr<UGridItemContextWidget> ContextWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	TObjectPtr<APawn> InstigatorPawn = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	TObjectPtr<APlayerController> InstigatorController = nullptr;
};

/**
 * Grid item action that is usually available via context's popup
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class GRIDINVENTORY_API UGridItemActionBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool ExecuteAction(UPARAM(ref) const FGridActionContext& ActionContext);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	FText ActionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory")
	TSubclassOf<UGridItemContextElement> ActionContextElementClass;

protected:
	virtual bool ExecuteActionInternal(const FGridActionContext& ActionContext);
};

UCLASS()
class UGridItemActionBP : public UGridItemActionBase
{
	GENERATED_BODY()

protected:
	virtual bool ExecuteActionInternal(const FGridActionContext& ActionContext) override;

	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName="ExecuteAction"), Category = "Grid Inventory")
	bool BP_ExecuteAction(const FGridActionContext& ActionContext);
};
