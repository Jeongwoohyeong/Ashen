// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class AAshenCharacter;
class USphereComponent;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_None UMETA(DisplayName = "None"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Shield UMETA(DisplayName = "Shield"),
	EIT_Armor UMETA(DisplayName = "Armor")
};

UCLASS()
class ASHEN_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	/*
		Functions
	*/
	/** Getter */
	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }
	/*
		Variables
	*/
	FORCEINLINE EItemType GetItemType() const { return ItemType; }

protected:
	/*
		Variables
	*/
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;	

private:
	/*
		Functions
	*/
	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/*
		Variables
	*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;	

	UPROPERTY()
	AAshenCharacter* Player;

	/** Type */ // 필요성 고민
	EItemType ItemType;
};
