// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item.h"
#include "Characters/AshenCharacter.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	ItemMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	Sphere->SetupAttachment(ItemMesh);
	Tags.Add(FName("Dummy"));
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::SphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::SphereEndOverlap);
}

void AItem::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Player) return;
	
	Player = Cast<AAshenCharacter>(OtherActor);
	if (Player)
	{
		Player->SetOverlapItem(this);
	}
}

void AItem::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	if (Player)
	{
		Player->SetOverlapItem(nullptr);
		Player = nullptr;
	}
}