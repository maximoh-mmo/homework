// Copyright 2025, Solar Corp. All right reserved


#include "Character/GridInventoryCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"
#include "Engine/LocalPlayer.h"

AGridInventoryCharacter::AGridInventoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	PlayerPocketsInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerPocketsInventory"));
}

void AGridInventoryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalViewingPlayerController()->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MainInputContext, 0);
		Subsystem->AddMappingContext(InventoryInputContext, 1);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AGridInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGridInventoryCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGridInventoryCharacter::Look);
	}
}

void AGridInventoryCharacter::AddMainInputContext()
{
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalViewingPlayerController()->GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(MainInputContext, 0);
	}
}

void AGridInventoryCharacter::RemoveMainInputContext()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalViewingPlayerController()->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MainInputContext);
	}
}

UInventoryComponent* AGridInventoryCharacter::GetPlayerPocketsInventoryComponent() const
{
	return PlayerPocketsInventoryComponent;
}

UGridContainerItem* AGridInventoryCharacter::GetBackpackItem() const
{
	return PlayerBackpack;
}

UGridContainerItem* AGridInventoryCharacter::GetRigItem() const
{
	return PlayerRig;
}

void AGridInventoryCharacter::SetBackpackItem(UGridContainerItem* InBackpackItem)
{
	PlayerBackpack = InBackpackItem;
}

void AGridInventoryCharacter::SetRigItem(UGridContainerItem* InRigItem)
{
	PlayerRig = InRigItem;
}

void AGridInventoryCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGridInventoryCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGridInventoryCharacter::AddInstancedComponentToCharacter(UActorComponent* InActorComponent)
{
	AddInstanceComponent(InActorComponent);
}
