// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/PlayerOverlay.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (PlayerOverlayClass && Controller)
		{
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}
