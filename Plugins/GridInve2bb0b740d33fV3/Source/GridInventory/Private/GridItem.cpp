// Copyright 2025, Solar Corp. All right reserved

#include "GridItem.h"

#include "Inventory.h"
#include "Actors/GridRenderTargetActor.h"
#include "Engine/SkinnedAssetCommon.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Interfaces/GridItemOwner.h"
#include "Misc/DataValidation.h"
#include "Subsystems/GridTextureGenerationSubsystem.h"

namespace
{
	void SetMipmapStreaming(UMaterialInterface* Material, ERHIFeatureLevel::Type FeatureLeve, bool IsMipmapStreamingEnabled)
	{
		TArray<UTexture*> Textures;
		Material->GetUsedTextures(Textures);

		if (!IsMipmapStreamingEnabled)
		{
			Material->SetForceMipLevelsToBeResident(true, true, -1.f);

			for (UTexture* Texture : Textures)
			{
				UTexture2D* tex2D = Cast<UTexture2D>(Texture);
				if (IsValid(tex2D))
				{
					tex2D->WaitForStreaming();
				}
			}
		}
		else
		{
			Material->SetForceMipLevelsToBeResident(true, false, -1.f);
		}
	}
	
	void SetSkeletalMeshCaptureParam(USkeletalMesh* SkeletalMesh, ERHIFeatureLevel::Type FeatureLeve, bool IsMipmapStreamingEnabled, bool CacheShaders)
	{
		for (FSkeletalMaterial& mat : SkeletalMesh->GetMaterials())
		{
			if (IsValid(mat.MaterialInterface))
			{
				/*if (CacheShaders)
				{
					mat.MaterialInterface->CacheShaders(EMaterialShaderPrecompileMode::Synchronous);
				}*/

				SetMipmapStreaming(mat.MaterialInterface, FeatureLeve, IsMipmapStreamingEnabled);
			}
		}
	}
	
	void SetStaticMeshCaptureParam(UStaticMesh* StaticMesh, ERHIFeatureLevel::Type FeatureLeve, bool IsMipmapStreamingEnabled, bool CacheShaders)
	{
		for (FStaticMaterial& mat : StaticMesh->GetStaticMaterials())
		{
			if (IsValid(mat.MaterialInterface))
			{
				/*if (CacheShaders)
				{
					mat.MaterialInterface->CacheShaders(EMaterialShaderPrecompileMode::Synchronous);
				}*/

				SetMipmapStreaming(mat.MaterialInterface, FeatureLeve, IsMipmapStreamingEnabled);
			}
		}
	}
}

bool UGridItem::CanInteractWithItem_Implementation(UGridItem* InOtherItem) const
{
	return CanStackWith(InOtherItem) && IsStackable() && GetCurrentStacks() < GetMaxStacks();
}

bool UGridItem::TryInteractWithItem_Implementation(UGridItem* InOtherItem)
{
	ensureAlwaysMsgf(false, TEXT("UGridItem don't have interact with each other"));
	return false;
}

bool UGridItem::CanStackWith(UGridItem* InOtherItem) const
{
	return ItemTags == InOtherItem->ItemTags && Dimensions == InOtherItem->Dimensions;
}

void UGridItem::InitItem()
{
	if (IsStackable())
	{
		SetCurrentStacks(InitialStacks);
	}
	
	BP_OnInit();
}

void UGridItem::GenerateIcon()
{
	// TODO: We have a very wasteful case, we capture the same image for items with the same class,
	// TODO: so if we have 2 grenade items, they both will make icon, but they should just have a common pointer to a single render target
	// TODO: so ideally we should have a subsystem or a manager for this thing, and this also should be done asynchronously 
	if (bUseGeneratedIcons)
	{
		UGridTextureGenerationSubsystem* textureGenerationSubsystem = GetWorld()->GetSubsystem<UGridTextureGenerationSubsystem>();
		if (!IsValid(textureGenerationSubsystem))
		{
			return;
		}
		
		AGridRenderTargetActor* iconGenerationActor = textureGenerationSubsystem->GetIconGenerationRenderActor();
		if (!IsValid(iconGenerationActor) || !ensureAlwaysMsgf(!iconGenerationActor->GetIsOccupied(), TEXT("Attempt to use occupied icon generation actor")))
		{
			return;
		}

		FRequestTextureCreationData requestData;
		requestData.renderSpecification = ItemViewRenderSpecification;

		if (ItemViewRenderSpecification.RenderTargetSizeOverride.X == 0 && ItemViewRenderSpecification.RenderTargetSizeOverride.Y == 0)
		{
			const float scale = ItemViewRenderSpecification.RenderTargetSizeScale;
			requestData.renderSpecification.RenderTargetSizeOverride = FIntPoint(GetGridCellSize() * Dimensions.X * scale, GetGridCellSize() * Dimensions.Y * scale);
		}
		
		if (ItemMeshType == EGridItemMeshType::Skeletal)
		{
			if (!ItemSkeletalMesh.IsValid())
			{
				USkeletalMesh* loadedMesh = ItemSkeletalMesh.LoadSynchronous();
				SetSkeletalMeshCaptureParam(loadedMesh, GetWorld()->GetFeatureLevel(), false, true);
				requestData.SkeletalMeshToCapture = loadedMesh;
			}
			else
			{
				requestData.SkeletalMeshToCapture = ItemSkeletalMesh.Get();
			}
		}
		else
		{
			if (!ItemMesh.IsValid())
			{
				UStaticMesh* loadedMesh = ItemMesh.LoadSynchronous();
				SetStaticMeshCaptureParam(loadedMesh, GetWorld()->GetFeatureLevel(), false, true);
				requestData.MeshToCapture = loadedMesh;
			}
			else
			{
				requestData.MeshToCapture = ItemMesh.Get();
			}
		}
		
		requestData.TextureCreatedCallback = FTextureCreatedDelegate::CreateUObject(this, &UGridItem::OnItemImageGeneratedCallback);
		
		iconGenerationActor->RequestTextureCreation(requestData);
	}
}

UTextureRenderTarget2D* UGridItem::GetItemGeneratedIcon() const
{
	return ItemGeneratedIcon;
}

UObject* UGridItem::GetItemOwnerInterfaceObject() const
{
	return ItemOwnerInterfaceObject;
}

void UGridItem::SetItemOwnerInterfaceObject(UObject* InOwner)
{
	if (!ensureAlwaysMsgf(IsValid(InOwner), TEXT("InOwner is nullptr")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(InOwner->Implements<UGridItemOwner>(), TEXT("InOwner doesn't implement UGridItemOwnerCommand")))
	{
		return;
	}

	ItemOwnerInterfaceObject = InOwner;
}

void UGridItem::SetGridCellSize(int32 InSize)
{
	GridCellSize = InSize;
}

FIntPoint UGridItem::GetDimensions() const
{
	return bIsRotated ? FIntPoint(Dimensions.Y, Dimensions.X) : Dimensions;
}

bool UGridItem::GetIsRotated() const
{
	return bIsRotated;
}

int32 UGridItem::GetGridCellSize() const
{
	return GridCellSize;
}

bool UGridItem::IsStackable() const
{
	return MaxStacks > 1;
}

bool UGridItem::IsMaxStacksReached() const
{
	return CurrentStacks == MaxStacks;
}

int32 UGridItem::GetMaxStacks() const
{
	return MaxStacks;
}

int32 UGridItem::GetCurrentStacks() const
{
	return CurrentStacks;
}

void UGridItem::ToggleRotate()
{
	bIsRotated = !bIsRotated;
}

void UGridItem::SetIsRotated(bool InIsRotated)
{
	bIsRotated = InIsRotated;
}

void UGridItem::SetCurrentStacks(int32 InStacks)
{
	if (!ensureAlwaysMsgf(InStacks >= 1, TEXT("InStacks is less than one")))
	{
		return;
	}

	CurrentStacks = InStacks;
}

void UGridItem::RequestWidgetUpdate()
{
	if (OnItemWidgetUpdateRequested.IsBound())
	{
		OnItemWidgetUpdateRequested.Broadcast();
	}
}

void UGridItem::OnItemImageGeneratedCallback(UTextureRenderTarget2D* InGeneratedTexture)
{
	ItemGeneratedIcon = InGeneratedTexture;

	if (ItemSkeletalMesh.IsValid())
	{
		SetSkeletalMeshCaptureParam(ItemSkeletalMesh.Get(), GetWorld()->GetFeatureLevel(), true, false);
	}
	if (ItemMesh.IsValid())
	{
		SetStaticMeshCaptureParam(ItemMesh.Get(), GetWorld()->GetFeatureLevel(), true, false);
	}
	
	if (OnItemIconGenerated.IsBound())
	{
		OnItemIconGenerated.Broadcast(ItemGeneratedIcon);
	}
}

void UGridContainerItem::InitItem()
{
	Super::InitItem();

	Inventory = NewObject<UInventory>(this);
	Inventory->InitInventory(ItemInventoryAsset);
}


bool UGridContainerItem::CanInteractWithItem_Implementation(UGridItem* InOtherItem) const
{
	if (Inventory->CanAddItem(InOtherItem))
	{
		return true;
	}
	
	return Super::CanInteractWithItem_Implementation(InOtherItem);
}

bool UGridContainerItem::TryInteractWithItem_Implementation(UGridItem* InOtherItem)
{
	if (!ensureAlwaysMsgf(CanInteractWithItem(InOtherItem), TEXT("Try interact with item was called when CanInteractWithItem is false")))
	{
		return false;
	}

	IGridItemOwner::Execute_RemoveItemCommand(InOtherItem->GetItemOwnerInterfaceObject(), InOtherItem);
	Inventory->TryAddItem(InOtherItem);
	return true;
}

UInventory* UGridContainerItem::GetInventory() const
{
	return Inventory;
}

#if WITH_EDITOR
EDataValidationResult UGridItem::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult result = UObject::IsDataValid(Context);

	if (bUseGeneratedIcons)
	{
		if (ItemMeshType == EGridItemMeshType::Skeletal && ItemSkeletalMesh.IsNull())
		{
			Context.AddError(FText::FromString("\nItemSkeletalMesh isn't assigned but bUseGeneratedIcons is checked, Please assign a valid skeletal mesh asset"));
		}
		else if (ItemMeshType == EGridItemMeshType::Static && ItemMesh.IsNull())
		{
			Context.AddError(FText::FromString("\nItemMeshType isn't assigned but bUseGeneratedIcons is checked, Please assign a valid static mesh asset"));
		}
	}

	if (IsStackable())
	{
		if (InitialStacks > MaxStacks)
		{
			Context.AddError(FText::FromString("\nInitialStacks is bigger than MaxStacks"));
		}
	}

	return result;
}
#endif