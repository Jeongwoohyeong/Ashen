// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon/Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;	

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseCharacter::Attack()
{
	
}

bool ABaseCharacter::IsAlive()
{
	if (Attributes)
	{
		return Attributes->GetHealthPercent() > 0.f;
	}

	UE_LOG(LogTemp, Warning, TEXT("IsAlive Function Error, There is No Attributes Component"));
	return false;
}

void ABaseCharacter::Dead()
{
	if (!DeathMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeathMontage NULL"));
		return;
	}
	PlayMontage(DeathMontage, "Death");
	HandleDeath();
}

void ABaseCharacter::HandleDeath()
{
	// 무기 삭제, 캐릭터 사라지는 시간이랑 동일하게 제거
	EquippedWeapon->SetLifeSpan(5.0f);
	// 메쉬 충돌 제거
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("asdas"));
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
	}
}

void ABaseCharacter::DodgeEnd()
{
	
}

void ABaseCharacter::AttackEnd()
{	
	
}

void ABaseCharacter::ComboAttackEnd()
{
	
}

void ABaseCharacter::SetWeaponBoxCollision(ECollisionEnabled::Type Collision)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(Collision);
		/*
		 *	무기 충돌 설정은 공격 시작과 종료에 호출
		 *	공격 시작 시 이전 피격 대상이 배열에 남아있지 않도록 비워줌
		 *	공격 종료 시 현재 피격 대상이 배열이 남아있지 않도록 비워줌
		 *	AttackEnd()에서 공격 종료 시에만 비워줘도 되지만, 피격처리에 관한 책임은 Weapon Class에 있다고 생각함(BoxOverlap함수에서 피격대상 trace와 데미지 적용)
		 *	BoxCollision이 활성화된 시간에만 IgnoreActors 수집
		 */
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::GetHit(const FVector ImpactPoint, const AActor* Hitter)
{
	/*
	 * 방향에 따라 hitreaction
	 * 생존여부 검사
	 * 체력바 띄우기
	 */
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());		
	}
}

void ABaseCharacter::PlayMontage(UAnimMontage* Montage, const FName& SectionName)
{
	if (Montage == nullptr) return;
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{		
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SectionName.ToString());
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector Impact(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector HitDirection = (Impact - GetActorLocation()).GetSafeNormal();

	const float CosT = FVector::DotProduct(Forward, HitDirection);
	double Angle = FMath::Acos(CosT);
	Angle = FMath::RadiansToDegrees(Angle);
	UE_LOG(LogTemp, Warning, TEXT("%f"), Angle);

	if (FVector::DotProduct(GetActorRightVector(), HitDirection) < 0.f)
	{
		Angle *= -1.f;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%f"), Angle);
	
	FName SectionName("FromFront");
	if (Angle > 45.f && Angle <= 135.f)
	{
		SectionName = FName("FromRight");
	}
	else if (Angle < -45.f && Angle >= -135.f)
	{
		SectionName = FName("FromLeft");
	}
	else if (Angle < -135.f || Angle > 135.f)
	{
		SectionName = FName("FromBack");
	}
	PlayMontage(HitReactMontage, SectionName);
}
