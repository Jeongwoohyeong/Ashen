// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASHEN_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Getter */
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	FORCEINLINE float GetStaminaPercent() const { return Stamina/ MaxStamina; }
	FORCEINLINE uint32 GetSouls() const { return Souls; }

	void UseStamina();
	void RegenStamina(const float& DeltaTime);
	void ReceiveDamage(float DamageAmount);

protected:	

private:
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float DodgeCost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	uint32 Souls;
		
};
