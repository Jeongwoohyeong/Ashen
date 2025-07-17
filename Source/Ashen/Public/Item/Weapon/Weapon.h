// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UENUM(BlueprintType)
enum EWeaponType : uint8
{
	EWT_None = 0 UMETA(DisplayName = "None"),
	EWT_OneHanded = 3 UMETA(DisplayName = "OneHanded"),
	EWT_TwoHanded = 99 UMETA(DisplayName = "OneHanded")
};

UCLASS()
class ASHEN_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
protected:
	virtual void BeginPlay() override;
	
public:
	/*
		Functions
	*/
	/** Getter */
	FORCEINLINE TEnumAsByte<EWeaponType> GetWeaponType() const { return WeaponType.GetValue(); }
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return Box; }
	
	void Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachToSocket(USceneComponent* Parent, FName SocketName);

	/*
		Variables
	*/
	UPROPERTY()
	TArray<AActor*> IgnoreActors;

protected:
	/*
		Functions
	*/
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/*
		Variables
	*/	
	
private:
	/*
		Functions
	*/
	
	/*
		Variables
	*/
	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BoxTraceStart;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BoxTraceEnd;
	
	UPROPERTY(EditAnywhere)
	uint8 bBoxTraceDebug : 1;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	TEnumAsByte<EWeaponType> WeaponType = EWT_OneHanded;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float DamageAmount = 20;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FVector BoxExtent = FVector(5.f);	
};
