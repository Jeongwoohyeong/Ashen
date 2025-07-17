// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ASHEN_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthBar(float HealthPercent);
	void SetStaminaBar(float StaminaPercent);
	void SetSouls(uint32 Souls);

protected:

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulText;
	
};
