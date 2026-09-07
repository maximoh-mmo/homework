// Copyright 2025, Solar Corp. All right reserved

#include "Actors/GridRenderTargetActor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetRenderingLibrary.h"


AGridRenderTargetActor::AGridRenderTargetActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootSceneComponent);

	MeshHolderSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshHolder"));
	MeshHolderSceneComponent->SetupAttachment(GetRootComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SpringArmComponent->SetupAttachment(MeshHolderSceneComponent);

	MeshCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MeshCapture"));
	MeshCaptureComponent->SetupAttachment(SpringArmComponent);

	// This transform scene is used to apply rotation. We offset the mesh so it’s centered on this component, enabling rotation around a common center regardless of the mesh's pivot.
	MeshTransformSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshTransformScene"));
	MeshTransformSceneComponent->SetupAttachment(MeshHolderSceneComponent);
	
	CapturedStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CapturedStaticMesh"));
	CapturedStaticMeshComponent->SetupAttachment(MeshTransformSceneComponent);

	CapturedSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CapturedSkeletalMesh"));
	CapturedSkeletalMeshComponent->SetupAttachment(MeshTransformSceneComponent);
}

void AGridRenderTargetActor::BeginPlay()
{
	Super::BeginPlay();

	CapturedStaticMeshComponent->bForceMipStreaming = true;
	CapturedStaticMeshComponent->bOverrideMinLOD = true;
	CapturedStaticMeshComponent->MinLOD = 0;
	CapturedSkeletalMeshComponent->bForceMipStreaming = true;
	CapturedSkeletalMeshComponent->bOverrideMinLod = true;
	CapturedSkeletalMeshComponent->MinLodModel = 0;
	
	// It is assumed that static and skeletal mesh has the same relative position
	MeshInitialRelativePosition = CapturedStaticMeshComponent->GetRelativeLocation();
	MeshTransformInitialRelativeRotation = MeshTransformSceneComponent->GetRelativeRotation();
	TargetArmInitialLength = SpringArmComponent->TargetArmLength;

	CapturingRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, RenderTargetSize.X, RenderTargetSize.Y);
	CapturingRenderTarget->LODGroup = TEXTUREGROUP_UI;

	// By default, tick is disabled, so performance is not affected when we don't need rendering
	MeshCaptureComponent->SetComponentTickEnabled(false);
	MeshCaptureComponent->TextureTarget = CapturingRenderTarget;
}

void AGridRenderTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Texture creation is split into 3 stages:
	// 1. Requesting (enqueueing)
	// 2. Setting position and capture settings — we need to give Unreal one frame to apply the updated position
	// 3. Capturing the final scene

	// TODO: This could be done asynchronously, do a profiling for this to test the difference
	if (CurrentRequestedData.bIsReadyForCapturing)
	{
#if WITH_EDITOR
		if (IsValid(CurrentRequestedData.SkeletalMeshToCapture))
		{
			if (!CapturedSkeletalMeshComponent->GetSkeletalMeshAsset()->IsReadyToRenderInThumbnail())
			{
				return;
			}
		}
#endif
		
		MeshCaptureComponent->CaptureScene();
		CurrentRequestedData.TextureCreatedCallback.ExecuteIfBound(CapturingRenderTarget);

		ResetCaptureConfigurations(true);

		CurrentRequestedData = {};
	}
	
	if (!RequestedTexturesCreations.IsEmpty())
	{
		RequestedTexturesCreations.Dequeue(CurrentRequestedData);

		if (IsValid(CurrentRequestedData.MeshToCapture))
		{
			SetStaticMeshToCapture(CurrentRequestedData.MeshToCapture);
		}
		else if (IsValid(CurrentRequestedData.SkeletalMeshToCapture))
		{
			SetSkeletalMeshToCapture(CurrentRequestedData.SkeletalMeshToCapture);
		}

		SetRenderTargetSpecification(CurrentRequestedData.renderSpecification);
		CurrentRequestedData.bIsReadyForCapturing = true;
	}
}

bool AGridRenderTargetActor::GetIsOccupied() const
{
	return bIsOccupied;
}

void AGridRenderTargetActor::SetIsOccupied(bool IsOccupied)
{
	if (!IsOccupied)
	{
		ResetCaptureConfigurations();
	}
	
	bIsOccupied = IsOccupied;
	MeshCaptureComponent->SetComponentTickEnabled(IsOccupied);
}

void AGridRenderTargetActor::SetRenderTargetSpecification(const FGridRenderTargetSpecification& InSpecification)
{
	if (InSpecification.RenderTargetSizeOverride.X != 0 && InSpecification.RenderTargetSizeOverride.Y != 0)
	{
		SetRenderTargetSize(InSpecification.RenderTargetSizeOverride.X, InSpecification.RenderTargetSizeOverride.Y);
	}
	
	SpringArmComponent->TargetArmLength += InSpecification.TargetArmLengthOffset;
	MeshTransformSceneComponent->AddRelativeRotation(InSpecification.MeshRelativeRotationOffset);

	UMeshComponent* meshComp = nullptr;
	
	if (IsValid(CapturedStaticMeshComponent->GetStaticMesh()))
	{
		meshComp = CapturedStaticMeshComponent;
	}
	else if (IsValid(CapturedSkeletalMeshComponent->GetSkeletalMeshAsset()))
	{
		meshComp = CapturedSkeletalMeshComponent;
	}
	
	FVector dif = meshComp->GetComponentLocation() - meshComp->Bounds.Origin;
	meshComp->SetWorldLocation(meshComp->GetComponentLocation() + dif);
	meshComp->AddWorldOffset(InSpecification.MeshPositionOffset);
}

void AGridRenderTargetActor::RenderTargetForInspectWidget(const FGridRenderTargetSpecification& InSpecification, int32 InWidth, int32 InHeight)
{
	SetRenderTargetSize(InWidth, InHeight);

	SpringArmComponent->TargetArmLength += InSpecification.InspectionTargetArmLengthOffset;
	MeshTransformSceneComponent->AddRelativeRotation(InSpecification.MeshRelativeRotationOffset);
	
	UMeshComponent* meshComp = nullptr;
	
	if (IsValid(CapturedStaticMeshComponent->GetStaticMesh()))
	{
		meshComp = CapturedStaticMeshComponent;
	}
	else if (IsValid(CapturedSkeletalMeshComponent->GetSkeletalMeshAsset()))
	{
		meshComp = CapturedSkeletalMeshComponent;
	}

	if (IsValid(meshComp))
	{
		FVector dif = meshComp->GetComponentLocation() - meshComp->Bounds.Origin;
		meshComp->SetWorldLocation(meshComp->GetComponentLocation() + dif);
		meshComp->AddWorldOffset(InSpecification.MeshPositionOffset);
	}
}

void AGridRenderTargetActor::SetRenderTargetSize(int32 InWidth, int32 InHeight)
{
	if (IsValid(CapturingRenderTarget) && CapturingRenderTarget->SizeX == InWidth && CapturingRenderTarget->SizeY == InHeight)
	{
		return;
	}
	
	CapturingRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, InWidth, InHeight);
	CapturingRenderTarget->LODGroup = TEXTUREGROUP_UI;
	MeshCaptureComponent->TextureTarget = CapturingRenderTarget;
}

void AGridRenderTargetActor::SetHoldingMouseButton(bool InIsHolding)
{
	bIsHoldingMouseButton = InIsHolding;
}

void AGridRenderTargetActor::AddTargetArmLength(float InLength)
{
	SpringArmComponent->TargetArmLength += InLength;
}

void AGridRenderTargetActor::SetStaticMeshToCapture(UStaticMesh* InStaticMesh)
{
	CapturedStaticMeshComponent->SetStaticMesh(InStaticMesh);
}

void AGridRenderTargetActor::SetSkeletalMeshToCapture(USkeletalMesh* InSkeletalMesh)
{
	CapturedSkeletalMeshComponent->SetSkeletalMesh(InSkeletalMesh);
}

void AGridRenderTargetActor::SetCaptureEveryFrame(bool InCapture)
{
	MeshCaptureComponent->bCaptureEveryFrame = InCapture;
}

void AGridRenderTargetActor::ResetCaptureConfigurations(bool ForeRecreateRenderTarget)
{
	SetStaticMeshToCapture(nullptr);
	SetSkeletalMeshToCapture(nullptr);

	if (ForeRecreateRenderTarget || (CapturingRenderTarget->SizeX != RenderTargetSize.X || CapturingRenderTarget->SizeY != RenderTargetSize.Y))
	{
		SetRenderTargetSize(RenderTargetSize.X, RenderTargetSize.Y);
	}
	
	CapturedStaticMeshComponent->SetRelativeLocation(MeshInitialRelativePosition);
	
	CapturedSkeletalMeshComponent->SetRelativeLocation(MeshInitialRelativePosition);

	MeshTransformSceneComponent->SetRelativeRotation(MeshTransformInitialRelativeRotation);
	
	SpringArmComponent->TargetArmLength = TargetArmInitialLength;
}

void AGridRenderTargetActor::RequestTextureCreation(const FRequestTextureCreationData& InData)
{
	RequestedTexturesCreations.Enqueue(InData);
}
