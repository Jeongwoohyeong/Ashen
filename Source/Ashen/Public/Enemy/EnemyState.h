#pragma once

UENUM(BlueprintType)
enum EEnemyState : uint8
{
	EES_Idle UMETA(DisplayName = "Idle"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Combat UMETA(DisplayName = "Combat"),
	EES_Dead UMETA(DisplayName = "Dead")
};