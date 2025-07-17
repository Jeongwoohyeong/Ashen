// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyState.h"
#include "Characters/BaseCharacter.h"
#include "HUD/HealthBarComponent.h"
#include "Enemy.generated.h"

class UHealthBarComponent;

UCLASS()
class ASHEN_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;	

protected:
	virtual void BeginPlay() override;

public:

protected:
	/*
	 *	Variables
	 */
	/** State */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyState> EnemyState = EEnemyState::EES_Idle;

private:
	/*
	 *	Functions
	 */
	/** Combat */
	virtual void GetHit(const FVector ImpactPoint, const AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Dead() override;
	void SetHPBarVisibility(bool value);

	/*
	 *	Variables
	 */
	/** Equipments */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AWeapon> WeaponClass;

	/** HUD */
	UPROPERTY(EditAnywhere, Category = "HUD")
	UHealthBarComponent* HealthBarWidget;	
};
