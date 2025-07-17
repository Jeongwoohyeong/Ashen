// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerAnimInstance.h"
#include "Characters/AshenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AshenCharacter = Cast<AAshenCharacter>(TryGetPawnOwner());
	if (AshenCharacter)
	{
		PlayerMovementComponent = AshenCharacter->GetCharacterMovement();
	}	
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (PlayerMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);
		bJump = PlayerMovementComponent->IsFalling();
		PlayerEquipment = AshenCharacter->GetEquipmentState();
		bLockon = AshenCharacter->IsLockon();
		// 락온된 대상과 캐릭터의 방향을 갱신
		if (bLockon)
		{
			Angle = AshenCharacter->GetCharacterDirection();
			/*
			 * 카메라랑 속력의 방향간의 각도
			 */
			//UE_LOG(LogTemp, Warning, TEXT("Angle : %f"), Angle);
		}
	}
}