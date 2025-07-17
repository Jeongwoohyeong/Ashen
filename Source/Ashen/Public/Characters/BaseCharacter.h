// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class ASHEN_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

public:		

protected:	
	/*
		Functions
	*/	
	/** Combat */
	void Attack();
	bool IsAlive();
	virtual void Dead();
	virtual void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void ComboAttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponBoxCollision(ECollisionEnabled::Type Collision);

	virtual void GetHit(const FVector ImpactPoint, const AActor* Hitter) override;	

	/** Montage Play */
	void PlayMontage(UAnimMontage* Montage, const FName& SectionName);

	/*
		Variables
	*/
	/** Montage */
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* AttackMontage;

	/** Equipments */
	UPROPERTY()
	AWeapon* EquippedWeapon;

	/** Components */
	UPROPERTY(EditAnywhere, Category = "Attribute")
	UAttributeComponent* Attributes;

private:
	/*
		Functions
	*/
	void DirectionalHitReact(const FVector ImpactPoint);	

	/*
		Variables
	*/
	/** Montage */
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* DeathMontage;
	
};
