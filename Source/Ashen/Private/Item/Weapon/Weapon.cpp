// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	Box = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxTraceStart = CreateDefaultSubobject<UStaticMeshComponent>(FName("BoxTraceStartComponent"));
	BoxTraceEnd = CreateDefaultSubobject<UStaticMeshComponent>(FName("BoxTraceEndComponent"));
	Box->SetupAttachment(GetItemMesh());
	BoxTraceStart->SetupAttachment(GetItemMesh());
	BoxTraceEnd->SetupAttachment(GetItemMesh());

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Box->SetGenerateOverlapEvents(true);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	
	SetOwner(NewOwner);	// 변수 Owner는 이미 존재해서 안됨 error C4458: declaration of 'Owner' hides class member
	SetInstigator(NewInstigator);	
	AttachToSocket(Parent, SocketName);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::AttachToSocket(USceneComponent* Parent, FName SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Parent,TransformRules, SocketName);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(GetOwner());
	ActorsToIgnore.AddUnique(this);
	FHitResult BoxHit;

	for (AActor* Actors : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actors);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxExtent,
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		(bBoxTraceDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None),
		BoxHit,
		true);

	if (BoxHit.GetActor())
	{
		ActorsToIgnore.AddUnique(BoxHit.GetActor());
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), DamageAmount, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		if (IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor()))
		{
			HitInterface->GetHit(BoxHit.ImpactPoint, GetOwner());
		}
	}
	
}
