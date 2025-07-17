// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class UCharacterMovementComponent;
class AAshenCharacter;
/**
 * 
 */
UCLASS()
class ASHEN_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	AAshenCharacter* AshenCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* PlayerMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Angle;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	uint8 bJump : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	uint8 bLockon : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TEnumAsByte<EEquipmentState> PlayerEquipment;
	
};
