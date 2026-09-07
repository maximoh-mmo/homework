// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "GridInventory/Public/Data/GridRenderTargetSpecification.h"

#include "GridRenderTargetActor.generated.h"

class UStaticMesh;
class USkeletalMesh;
class UTexture2D;
class USpringArmComponent;
class USceneCaptureComponent2D;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UTextureRenderTarget2D;

DECLARE_DELEGATE_OneParam(FTextureCreatedDelegate, UTextureRenderTarget2D*)

USTRUCT()
struct GRIDINVENTORY_API FRequestTextureCreationData
{
	GENERATED_BODY()

	FGridRenderTargetSpecification renderSpecification;

	UPROPERTY()
	TObjectPtr<UStaticMesh> MeshToCapture = nullptr;

	UPROPERTY()
	TObjectPtr<USkeletalMesh> SkeletalMeshToCapture = nullptr;

	FTextureCreatedDelegate TextureCreatedCallback;
	
	// If true it means that settings from this struct was set to render target on previous frame, and it can be captured in a current frame
	bool bIsReadyForCapturing = false;
};

UCLASS()
class GRIDINVENTORY_API AGridRenderTargetActor : public AActor
{
	GENERATED_BODY()

public:
	AGridRenderTargetActor();

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	bool GetIsOccupied() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetIsOccupied(bool IsOccupied);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetRenderTargetSpecification(const FGridRenderTargetSpecification& InSpecification);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void RenderTargetForInspectWidget(const FGridRenderTargetSpecification& InSpecification, int32 InWidth, int32 InHeight);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetRenderTargetSize(int32 InWidth, int32 InHeight);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetHoldingMouseButton(bool InIsHolding);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddTargetArmLength(float InLength);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetStaticMeshToCapture(UStaticMesh* InStaticMesh);
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetSkeletalMeshToCapture(USkeletalMesh* InSkeletalMesh);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetCaptureEveryFrame(bool InCapture);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Grid Inventory")
	void CombineInspectingObjectMouseRotation(const FVector2D& InMouseDelta);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void ResetCaptureConfigurations(bool ForeRecreateRenderTarget = false);

	void RequestTextureCreation(const FRequestTextureCreationData& InData);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USceneComponent> RootSceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USceneComponent> MeshHolderSceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USpringArmComponent> SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USceneCaptureComponent2D> MeshCaptureComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USceneComponent> MeshTransformSceneComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UStaticMeshComponent> CapturedStaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<USkeletalMeshComponent> CapturedSkeletalMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Grid Inventory")
	FIntPoint RenderTargetSize = FIntPoint(512, 512);

	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UTextureRenderTarget2D> CapturingRenderTarget = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory")
	bool bIsHoldingMouseButton = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UTexture2D> TestTexture = nullptr;
	
private:
	FVector MeshInitialRelativePosition = FVector::ZeroVector;
	FRotator MeshTransformInitialRelativeRotation = FRotator::ZeroRotator;
	float TargetArmInitialLength = 0.f;;
	
	bool bIsOccupied = false;
	
	TQueue<FRequestTextureCreationData> RequestedTexturesCreations;
	FRequestTextureCreationData CurrentRequestedData;
};
