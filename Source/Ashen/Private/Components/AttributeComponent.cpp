// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent() : Health(0.f), Stamina(0.f), Souls(0.f)
{	
	PrimaryComponentTick.bCanEverTick = false;	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UAttributeComponent::UseStamina()
{
	Stamina = FMath::Clamp(Stamina - DodgeCost, 0.f, MaxStamina);
}

void UAttributeComponent::RegenStamina(const float& DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + (StaminaRegenRate * DeltaTime), 0.f, MaxStamina);
}

void UAttributeComponent::ReceiveDamage(float DamageAmount)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
}