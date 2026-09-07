// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Events/GridGameplayMessageSubsystem.h"

#include "GridItemWidget.generated.h"

struct FGridInventoryDefinition;
struct FGridItemDragEventData;

class UMaterialInterface;
class USizeBox;
class UImage;
class UGridItem;
class UGridPayload;
class UInventoryAsset;
class UTexture2D;
class UTextureRenderTarget2D;
class UTextBlock;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragDropCreated, UGridPayload*, Payload);

UCLASS()
class GRIDINVENTORY_API UGridItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Grid Inventory")
	FOnDragDropCreated OnDragDropCreated;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool GetIsWidgetRotated() const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	float GetWidgetWidth() const;
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	float GetWidgetHeight() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridItem* GetItem() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetWidgetSize(float InWidth, float InHeight);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetupItem(UGridItem* InItem, const UInventoryAsset* InInventoryAsset);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetupItemForEquipSlot(UGridItem* InItem, float InWidth, float InHeight);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetupWidgetForDragOperation(const UGridItem* const InItem, float InWidth, float InHeight);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void ToggleRotateForDragOperation();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void UpdateItemDisplay();
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void BroadcastDragDropCreated(UGridPayload* InPayload);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UImage> ItemImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<USizeBox> ItemSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UTextBlock> ItemStacksTextBlock = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "Grid Inventory")
	TObjectPtr<UBorder> BackgroundBorder = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	TObjectPtr<UMaterialInterface> IconMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	FGameplayTag DragStartedEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	FGameplayTag DragEndedEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	FGameplayTag DragSplitStartedEventTag;

	UPROPERTY(EditDefaultsOnly, Category = "Grid Inventory")
	FGameplayTag DragSplitEndedEventTag;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory")
	FLinearColor InitialBackgroundColor = FLinearColor::White;

private:
	void SetWidgetParams(const UGridItem* const InItem, float InWidth, float InHeight);
	void SetCorrectImageSize() const;

	UFUNCTION()
	void OnItemIconGenerated(UTextureRenderTarget2D* InGeneratedIcon);

	UFUNCTION()
	void OnItemDragStarted(FGameplayTag InEventTag, const FGridItemDragEventData& InEvent);
	UFUNCTION()
	void OnItemDragEnded(FGameplayTag InEventTag, const FGridItemDragEventData& InEvent);
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess), Category = "Grid Inventory")
	TObjectPtr<UGridItem> Item = nullptr;

	float WidgetWidth = 0.f;
	float WidgetHeight = 0.f;

	FIntPoint IconSize = FIntPoint(0, 0);

	bool bIsRotated = false;

	FGridGameplayMessageListenerHandle DragStartedListener;
	FGridGameplayMessageListenerHandle DragEndedListener;
	FGridGameplayMessageListenerHandle DragSplitStartedListener;
	FGridGameplayMessageListenerHandle DragSplitEndedListener;
};
