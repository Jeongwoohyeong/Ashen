// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AshenCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "Components/AttributeComponent.h"
#include "Item/Weapon/Weapon.h"



AAshenCharacter::AAshenCharacter()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	ViewCamera->SetupAttachment(CameraBoom);
	CameraBoom->TargetArmLength = 300.0f;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	CameraBoom->bUsePawnControlRotation = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AAshenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerOverlay && Attributes)
	{
		Attributes->RegenStamina(DeltaTime);
		PlayerOverlay->SetStaminaBar(Attributes->GetStaminaPercent());
	}
	
	if (LockonTarget)
	{
		SetFocusAtLockonTarget(DeltaTime);
		CheckTargetDistance();			
	}	
}

void AAshenCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	if(UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAshenCharacter::Move);
		Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AAshenCharacter::Turn);
		Input->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AAshenCharacter::LookUp);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAshenCharacter::Jump);
		Input->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AAshenCharacter::StartWalk);
		Input->BindAction(WalkAction, ETriggerEvent::Completed, this, &AAshenCharacter::StopWalk);
		Input->BindAction(InterAction, ETriggerEvent::Triggered, this, &AAshenCharacter::InterActionKeyPress);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAshenCharacter::Attack);
		Input->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AAshenCharacter::Dodge);
		Input->BindAction(LockonAction, ETriggerEvent::Triggered, this, &AAshenCharacter::Lockon);
	}
}

void AAshenCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(MappingContext, 0);
		}

		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PC->GetHUD()))
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes)
			{
				PlayerOverlay->SetHealthBar(Attributes->GetHealthPercent());
				PlayerOverlay->SetStaminaBar(Attributes->GetStaminaPercent());
				PlayerOverlay->SetSouls(Attributes->GetSouls());
			}
		}
	}
	if (PunchMontage)
	{
		AttackMontage = PunchMontage;
	}
	bLockon = false;
}

double AAshenCharacter::GetCharacterDirection() const
{
	/*
	 * GetVelocity.safenormal은 world기준의 방향벡터 반환. 캐릭터가 월드 x좌표로 향하면 x = 1.0
	 * viewcamera의 포워드벡터도 동일
	 * 타겟과 나 사이의 벡터를 x 기저벡터로 하는 새로운 회전 행렬 R 생성
	 * R과 velocity를 이용해서 새로운 회전행렬 R' 생성
	 * R'의 역행렬로 회전값 계산
	 */
	if (!LockonTarget) return 0.f;
	
	const float Theta = UKismetAnimationLibrary::CalculateDirection(GetVelocity(), GetControlRotation());
	return Theta;
}

void AAshenCharacter::SetOverlapItem(AItem* Item)
{
	if (Item)
	{
		OverlapItem = Item;
	}	
}

void AAshenCharacter::DodgeEnd()
{
	// 구르기 상태에서 원래 상태로 전환
	ActionState = EActionState::EAS_Idle;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AAshenCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Idle;
}

void AAshenCharacter::ComboAttackEnd()
{
	GetWorldTimerManager().SetTimer(AttackEndTimer, this, &AAshenCharacter::AttackMontageStop, 1.f, false, 0.5f);
}

void AAshenCharacter::SetCharacterMeshCollsion(ECollisionEnabled::Type Collision)
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(Collision);
	}
}

void AAshenCharacter::AttachToSpine()
{
	EquippedWeapon->AttachToSocket(GetMesh(), FName("SpineSocket"));
}

void AAshenCharacter::AttachToRightHand()
{
	EquippedWeapon->AttachToSocket(GetMesh(), FName("RightHandSocket"));
}

void AAshenCharacter::Move(const FInputActionValue& Value)
{
	/*
		공격 중에는 이동 불가능, 죽으면 컨트롤러 해제 설정으로 이동 불가능하게 할 예정
	*/
	if (ActionState == EActionState::EAS_Attacking) return;
	
	const FVector2D MoveScale = Value.Get<FVector2D>();
	if (GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawDirection(0.f, Rotation.Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawDirection).GetUnitAxis(EAxis::Y);
		AddMovementInput(Forward, MoveScale.Y);
		AddMovementInput(Right, MoveScale.X);
	}
}

void AAshenCharacter::Turn(const FInputActionValue& Value)
{
	const float TurnValue = Value.Get<float>();
	if (GetController() && TurnValue)
	{
		AddControllerYawInput(TurnValue * MouseSensitivity);
	}
}

void AAshenCharacter::LookUp(const FInputActionValue& Value)
{
	const float LookUpValue = Value.Get<float>();
	if (GetController() && LookUpValue)
	{
		AddControllerPitchInput(LookUpValue * MouseSensitivity);
	}
}

void AAshenCharacter::StartWalk(const FInputActionValue& Value)
{
	if (GetController() && Value.Get<bool>())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AAshenCharacter::StopWalk(const FInputActionValue& Value)
{
	if (GetController() && !Value.Get<bool>())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AAshenCharacter::InterActionKeyPress(const FInputActionValue& Value)
{
	if (!GetController() || !Value.Get<bool>()) return;
	
	if (OverlapItem && EquippedWeapon == nullptr)
	{
		EquippedWeapon = Cast<AWeapon>(OverlapItem);
		if (EquippedWeapon)
		{
			EquippedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
			SetEquipmentState();
		}
	}
	else if (EquippedWeapon && !GetCharacterMovement()->IsFalling())
	{
		if (ActionState == EActionState::EAS_Idle && EquipmentState != EEquipmentState::ECS_NoEquipped)
		{
			Super::PlayMontage(EquipMontage, FName("DisArm"));
			AttackMontage = PunchMontage;
			EquipmentState = EEquipmentState::ECS_NoEquipped;
		}
		else if (ActionState == EActionState::EAS_Idle && EquipmentState == EEquipmentState::ECS_NoEquipped)
		{
			PlayMontage(EquipMontage, FName("Arm"));
			SetEquipmentState();
		}		
	}
	// 무장해제(방패), 아이템 줍기, 보스방 입장, 화톳불
}

void AAshenCharacter::Dodge(const FInputActionValue& Value)
{
	if (!GetController() || !Value.Get<bool>() || GetCharacterMovement()->IsFalling() || ActionState == EActionState::EAS_Dodging) return;
	
	PlayMontage(DodgeMontage, FName("Dodge"));
	GetCharacterMovement()->MaxWalkSpeed = DodgeSpeed;
	Attributes->UseStamina();
	ActionState = EActionState::EAS_Dodging;
	
	/*
		구르기 상태 전환, 충돌반옹 끄기(애님 노티파이 설정해놓음 콜리전 어케할지 결정해야함)
	*/	
}

void AAshenCharacter::Lockon(const FInputActionValue& Value)
{
	if (!GetController() || !Value.Get<bool>()) return;

	// 이미 락온된 대상이 존재하면 락온 해제
	if (LockonTarget)
	{
		LockonTarget = nullptr;
		bLockon = false;
		bUseControllerRotationYaw = false;
		return;
	}
	
	if (ViewCamera)
	{
		TArray<AActor*> ActorToIgnores;
		FHitResult HitResult;
		// 카메라가 바라보는 방향의 전방벡터
		const FVector Start = ViewCamera->GetComponentLocation();
		const FVector End = Start + (ViewCamera->GetForwardVector() * 1300.f);
		/*
		 * end point에 sphere trace 설정, hit actor가 enemy이면 lockon, 한번 더 누르면 lock on 해제
		 * sphere trace에서 warldstatic은 무시, world dynamic은 trace하도록 설정
		 */
		UKismetSystemLibrary::SphereTraceSingle(
			this,
			Start,
			End,
			25.f,
			TraceTypeQuery1,
			false,
			ActorToIgnores,
			(bShowSphereTraceDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None),
			HitResult,
			true);
		AActor* HitActor = HitResult.GetActor();

		// 더미액터로 테스트 중
		if (HitActor && HitActor->ActorHasTag(FName("Dummy")))
		{
			LockonTarget = HitActor;			
			bLockon = true;
			bUseControllerRotationYaw = true;			
		}
	}
}

void AAshenCharacter::Attack(const FInputActionValue& Value)
{
	//if (ActionState != EActionState::EAS_Idle) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	ActionState = EActionState::EAS_Attacking;
	
	/*
		공격 애님 나누지 말고 연속으로 되게 설정
		특정 지점에서 ComboAttackEnd 호출해서 애님 종료 타이머 실행
		타이머 실행 중 공격 입력되면 타이머 해제해서 다음 공격 모션 진행되도록 설정
	*/
	if (GetWorldTimerManager().IsTimerActive(AttackEndTimer) && Value.Get<bool>())
	{
		GetWorldTimerManager().ClearTimer(AttackEndTimer);
	}
	else if (AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		return;
	}	
	
	switch (EquipmentState)
	{
	case EEquipmentState::ECS_NoEquipped:
		PlayMontage(AttackMontage, PunchSectionName[SectionIndex]);
		break;
	case EEquipmentState::ECS_OneHandedWeapon:
		PlayMontage(AttackMontage, OneHandedSectionName[SectionIndex]);
		break;
	case EEquipmentState::ECS_TwoHandedWeapon:
		PlayMontage(AttackMontage, TwoHandedSectionName[SectionIndex]);
		break;
	default:
		break;
	}
	SectionIndex = (SectionIndex + 1) % NumberOfAttack;
	/*
		적 상태에 따라서 공격 애니 재생
		1. 일반교전 - 일반 공격
		2. 백어택 - 후방 일정 각도에서 공격 시 백어택 발생
		3. 그로기 -적 그로기 상태일 시 강공격

		캐릭터 무장 상태 별 공격
		1. 맨손 (+ 방패 있을 때 어떻게 ?)
		2. 한손검
		3. 양손검

		점프 공격
	*/
}

void AAshenCharacter::Jump()
{
	if (ActionState != EActionState::EAS_Idle) return;

	Super::Jump();
}

void AAshenCharacter::SetEquipmentState()
{
	if (EquippedWeapon == nullptr) return;
	
	const uint8 WeaponType = EquippedWeapon->GetWeaponType();
	NumberOfAttack = WeaponType;
	switch (WeaponType)
	{
	case EWeaponType::EWT_OneHanded :
		AttackMontage = OneHandWeaponMontage;
		EquipmentState = EEquipmentState::ECS_OneHandedWeapon;
		break;
	case EWeaponType::EWT_TwoHanded :
		AttackMontage = TwoHandWeaponMontage;
		EquipmentState = EEquipmentState::ECS_TwoHandedWeapon;
	default:
		break;
	}
}

void AAshenCharacter::AttackMontageStop()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopSlotAnimation();
		SectionIndex = 0;
		ActionState = EActionState::EAS_Idle;
	}
}

void AAshenCharacter::SetFocusAtLockonTarget(const float& DeltaTime)
{
	const FVector CameraLocation = ViewCamera->GetComponentLocation();
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, LockonTarget->GetActorLocation());
	const FRotator InterpolatedRotation = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, 15.f);
	GetController()->SetControlRotation(InterpolatedRotation);	
}

void AAshenCharacter::CheckTargetDistance()
{
	const FVector TargetDistance = LockonTarget->GetActorLocation() - GetActorLocation();
	// 설정 거리 이상이면 락온 해제
	if (TargetDistance.Size() > 1000.f)
	{
		LockonTarget = nullptr;
		bLockon = false;
		bUseControllerRotationYaw = false;
	}
}

