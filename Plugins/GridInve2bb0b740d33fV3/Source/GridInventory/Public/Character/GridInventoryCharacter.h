// Copyright 2025, Solar Corp. All right reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/GridCharacterInterface.h"
#include "UObject/Object.h"

#include "GridInventoryCharacter.generated.h"

struct FInputActionValue;

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UInventoryComponent;
class UGridContainerItem;


UCLASS()
class GRIDINVENTORY_API AGridInventoryCharacter : public ACharacter, public IGridCharacterInterface
{
	GENERATED_BODY()
	
public:
	AGridInventoryCharacter();
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UInventoryComponent* GetPlayerPocketsInventoryComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridContainerItem* GetBackpackItem() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	UGridContainerItem* GetRigItem() const;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetBackpackItem(UGridContainerItem* InBackpackItem);

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void SetRigItem(UGridContainerItem* InRigItem);

protected:
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void RemoveMainInputContext();
	
	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddMainInputContext();
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid Inventory")
	TObjectPtr<UInventoryComponent> PlayerPocketsInventoryComponent = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Grid Inventory")
	void AddInstancedComponentToCharacter(UActorComponent* InActorComponent);

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1P = nullptr;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ALlowPrivateAccess = "true"), Category = "Grid Inventory")
	TObjectPtr<UInputMappingContext> MainInputContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ALlowPrivateAccess = "true"), Category = "Grid Inventory")
	TObjectPtr<UInputMappingContext> InventoryInputContext = nullptr;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction = nullptr;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction = nullptr;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY()
	TObjectPtr<UGridContainerItem> PlayerBackpack = nullptr;

	UPROPERTY()
	TObjectPtr<UGridContainerItem> PlayerRig = nullptr;
};
