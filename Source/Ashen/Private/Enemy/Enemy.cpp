// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Item/Weapon/Weapon.h"

#include "Ashen/DebugMacros.h"

AEnemy::AEnemy()
{
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>("HealthBar");
	HealthBarWidget->SetupAttachment(GetMesh());
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetVisibility(false);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();	

	if (WeaponClass && GetWorld())
	{
		AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		EquippedWeapon = DefaultWeapon;
		EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	}
	if (HealthBarWidget)
	{
		if (Attributes)
		{
			HealthBarWidget->SetHealthBarPercent(Attributes->GetHealthPercent());			
		}		
	}
	
}

void AEnemy::GetHit(const FVector ImpactPoint, const AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);

	if (IsAlive())
	{
		SetHPBarVisibility(true);
		/*
		 *	피격 방향을 보게 하고 일정 거리 내의 플레이어 추적, 전투사거리 내에 들어오면 전투 시작 - AI 구현 해야함
		 */
	}
	else
	{
		SetHPBarVisibility(false);
		Dead();
		/*
		 *	죽음모션 설정
		 */
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthBarPercent(Attributes->GetHealthPercent());
	}

	return DamageAmount;
}

void AEnemy::Dead()
{
	Super::Dead();
	EnemyState = EEnemyState::EES_Dead;
	SetLifeSpan(5.0f);
	/*
	 *	BaseCharater에 구현할 것 - 죽음 모션, 컨트롤러 해제(적과 플레이어 동일하게 가능한지 모르겠음)
	 *	죽고나서 일정 시간있다가 사라지도록 하기 - 플레이어도 동일하게 할지 고민
	 *	소울 스폰 설정, 소울 만들어야함
	 */
}

void AEnemy::SetHPBarVisibility(bool value)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(value);
	}
}
