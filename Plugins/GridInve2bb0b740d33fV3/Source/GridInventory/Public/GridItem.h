// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "GridInventory/Public/Data/GridRenderTargetSpecification.h"
#include "GridInventory/Public/Actions/GridItemActionBase.h"

#include "GridItem.generated.h"

class UInventoryAsset;
class UInventory;
class UTexture2D;
class UTextureRenderTarget2D;

UENUM(Blueprintable, BlueprintType)
enum class EGridItemMeshType : uint8
{
	Static,
	Skeletal
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemIconGenerated, UTextureRenderTarget2D*, GeneratedIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemWidgetUpdateRequested);

UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class GRIDINVENTORY_API UGridItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnItemIconGenerated OnItemIconGenerated;

	UPROPERTY(BlueprintAssignable)
	FOnItemWidgetUpdateRequested OnItemWidgetUpdateRequested;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Inventory")
	bool CanInteractWithItem(UGridItem* InOtherItem) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grid Inventory")
	bool TryInteractWithItem(UGridItem* InOtherItem);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	virtual bool CanStackWith(UGridItem* InOtherItem) const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	virtual void InitItem();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void GenerateIcon();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UTextureRenderTarget2D* GetItemGeneratedIcon() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UObject* GetItemOwnerInterfaceObject() const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	FIntPoint GetDimensions() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool GetIsRotated() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	int32 GetGridCellSize() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool IsStackable() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool IsMaxStacksReached() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	int32 GetMaxStacks() const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	int32 GetCurrentStacks() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetItemOwnerInterfaceObject(UObject* InOwner);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetGridCellSize(int32 InSize);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void ToggleRotate();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetIsRotated(bool InIsRotated);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetCurrentStacks(int32 InStacks);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void RequestWidgetUpdate();

	virtual bool CanInteractWithItem_Implementation(UGridItem* InOtherItem) const;
	virtual bool TryInteractWithItem_Implementation(UGridItem* InOtherItem);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(MultiLine=true, ExposeOnSpawn))
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	FGameplayTagContainer ItemTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	FIntPoint Dimensions = FIntPoint(1, 1);

	// If true an icon for this item will be generated automatically on Item's init
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	bool bUseGeneratedIcons = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	EGridItemMeshType ItemMeshType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(EditCondition = "ItemMeshType == EGridItemMeshType::Static", EditConditionHides), meta=(ExposeOnSpawn))
	TSoftObjectPtr<UStaticMesh> ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(EditCondition = "ItemMeshType == EGridItemMeshType::Skeletal", EditConditionHides), meta=(ExposeOnSpawn))
	TSoftObjectPtr<USkeletalMesh> ItemSkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Actions", meta=(ExposeOnSpawn))
	TArray<TSubclassOf<UGridItemActionBase>> ItemActionClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Actions", meta=(ExposeOnSpawn))
	TSubclassOf<UGridItemActionBase> ItemDoubleClickActionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View Rendering", meta=(ExposeOnSpawn))
	FGridRenderTargetSpecification ItemViewRenderSpecification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stacking", meta=(ExposeOnSpawn, ClampMin = 1))
	int32 MaxStacks = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stacking", meta=(ExposeOnSpawn, ClampMin = 1))
	int32 InitialStacks = 1;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Init"), Category = "Grid Inventory")
	void BP_OnInit();

	// Item owner that implements IGridItemOwnerCommand
	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UObject> ItemOwnerInterfaceObject = nullptr;
	
private:
	UFUNCTION()
	void OnItemImageGeneratedCallback(UTextureRenderTarget2D* InGeneratedTexture);

	UPROPERTY()
	bool bIsRotated = false;

	// This is used only when we generate thumbnails dynamically
	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> ItemGeneratedIcon = nullptr;

	// This is used to set the correct widget size for the dragging widget, when we didn't use OnDrop event
	// Usually, this value is set from inventory asset, when we add item to inventory in this function UInventory::TryAddItem
	// But not all places use this initialization. For example, when you create an item and put it in an equip slot.
	// In this scenario, the equip slot widget needs this size to create the correct dragging widget.
	// This slot didn't get the widget size inside OnDrop, because there was no dropping. So in cases like that you need to set GridCellSize inside the equip slot
	UPROPERTY()
	int32 GridCellSize = 50;
	
	UPROPERTY()
	int32 CurrentStacks = 1;
};

UCLASS()
class UGridContainerItem : public UGridItem
{
	GENERATED_BODY()

public:
	virtual void InitItem() override;
	virtual bool CanInteractWithItem_Implementation(UGridItem* InOtherItem) const override;
	virtual bool TryInteractWithItem_Implementation(UGridItem* InOtherItem) override;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UInventory* GetInventory() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Inventory", meta=(ExposeOnSpawn))
	TObjectPtr<UInventoryAsset> ItemInventoryAsset = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UInventory> Inventory = nullptr;
};
