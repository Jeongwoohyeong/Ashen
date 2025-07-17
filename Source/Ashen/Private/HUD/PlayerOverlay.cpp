// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetHealthBar(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
}

void UPlayerOverlay::SetStaminaBar(float StaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercent);
	}
}

void UPlayerOverlay::SetSouls(uint32 Souls)
{
	if (SoulText)
	{		
		SoulText->SetText(FText::FromString(FString::Printf(TEXT("%u"), Souls)));
	}
}
