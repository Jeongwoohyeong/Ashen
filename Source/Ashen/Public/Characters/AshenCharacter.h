// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterState.h"
#include "Engine/TimerHandle.h"
#include "AshenCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UPlayerOverlay;

UCLASS()
class ASHEN_API AAshenCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAshenCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	/*
		Functions
	*/
	/** Getter */
	FORCEINLINE uint8 IsLockon() const { return bLockon; }
	FORCEINLINE EEquipmentState GetEquipmentState() const { return EquipmentState; }
	double GetCharacterDirection() const;

	/** Equipment */
	void SetOverlapItem(AItem* Item);

protected:
	/*
		Functions
	*/
	/** Combat */
	virtual void DodgeEnd() override;
	virtual void AttackEnd() override;
	void ComboAttackEnd() override;

	UFUNCTION(BlueprintCallable)
	void SetCharacterMeshCollsion(ECollisionEnabled::Type Collision);
	
	UFUNCTION(BlueprintCallable)
	virtual void AttachToSpine();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttachToRightHand();

	/*
		Variables
	*/
	/** Combat */
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> PunchSectionName;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> OneHandedSectionName;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> TwoHandedSectionName;

	/** State */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEquipmentState> EquipmentState = EEquipmentState::ECS_NoEquipped;

	UPROPERTY(EditAnywhere)
	uint8 bShowSphereTraceDebug : 1;
	
private:
	//  보조장비(방패), 패링, 죽음
	/*
		Functions
	*/
	/** Input */
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void StartWalk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void InterActionKeyPress(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void Lockon(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	virtual void Jump() override;	
	
	/** Combat */
	void SetEquipmentState();
	void AttackMontageStop();
	void SetFocusAtLockonTarget(const float& DeltaTime);
	void CheckTargetDistance();	

	/*
		Variables
	*/	
	/** Inputs */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookUpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InterAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LockonAction;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MouseSensitivity = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 700.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float DodgeSpeed = 450.f;

	/** Montage */
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* DodgeMontage;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* PunchMontage;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* OneHandWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* TwoHandWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* LeafAttackMontage;

	FTimerHandle AttackEndTimer;	

	/** Components */
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* ViewCamera;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	/** Equipments */
	UPROPERTY()
	AItem* OverlapItem;

	/** Combat */
	uint8 SectionIndex = 0;
	uint8 NumberOfAttack = 2;
	uint8 bLockon : 1;

	UPROPERTY()
	AActor* LockonTarget;

	/** State */
	EActionState ActionState = EActionState::EAS_Idle;

	/** HUD */
	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};

