#pragma once

UENUM(BlueprintType)
enum EEquipmentState : uint8
{
	ECS_NoEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_OneHandedWeapon UMETA(DisplayName = "OneHandedWeapon"),
	ECS_OneHandedWeaponShield UMETA(DisplayName = "OneHandedWeaponShield"),
	ECS_TwoHandedWeapon UMETA(DisplayName = "TwoHandedWeapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Idle UMETA(DisplayName = "Idle"),
	EAS_Dodging UMETA(DisplayName = "Dodging"),
	EAS_Jumping UMETA(DisplayName = "Jumping"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Dead UMETA(DisplayName = "Dead")
};