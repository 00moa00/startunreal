// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossRobot.h"
#include "Components/CapsuleComponent.h"
#include "Physics/FPSCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/FPSWidgetComponent.h"
#include "UI/HeartWidget.h"
#include "Animation/FPSAnimInstance.h"
#include "Monster/MonsterDataComponent.h"
#include "AI/BossAIController.h"
#include "Character/Sound.h"
#include "Components/SphereComponent.h"
#include "Monster/BossComboActionData.h"

#include "Particle/ParticleNiagara.h"
#include "NiagaraComponent.h"
#include "Character/FPSPlayerController.h"

#include "Monster/MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/CharacterCameraShake.h"


ABossRobot::ABossRobot()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MONSTER);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABossRobot::OnOverlapBegin);
	GetCapsuleComponent()->SetCapsuleRadius(80.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
	//GetCapsuleComponent()->SetWorldScale3D(FVector(200.f, 200.f, NewScale));
	GetCapsuleComponent()->IsInBlueprint();

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->JumpZVelocity = 0.f;

	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //PlacedInWorldOrSpawned 지정해서 배치된 폰 혹은 스폰 액터를 통해 등장시킨 폰 모두 ai 컨트롤러에 통제받는다

	AttackState = EJumpAttackState::Jump;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.Skeleton'/Game/ParagonCrunch/Characters/Heroes/Crunch/Meshes/Crunch_Skeleton.Crunch_Skeleton'"));
	//if (CharacterMeshRef.Object)
	//{
	//	GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	//}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GTFO/Animation/AM_BossDead.AM_BossDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GTFO/Animation/AM_BossComboAttack.AM_BossComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RotationAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GTFO/Animation/AM_RotationAttack.AM_RotationAttack'"));
	if (RotationAttackMontageRef.Object)
	{
		RotationAttackMontage = RotationAttackMontageRef.Object;
	}

}

void ABossRobot::BeginPlay()
{
	Super::BeginPlay();

	AnimInst = Cast<UFPSAnimInstance>(GetMesh()->GetAnimInstance());
	Stat->SetMonsterType("Boss");
	Stat->OnHpZero.AddUObject(this, &ABossRobot::SetDead);

}

void ABossRobot::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}


void ABossRobot::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepHitResult);

}


float ABossRobot::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);

	ASound* AMonsterSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
	AMonsterSound->SetSoundType(ESoundScale::Scale3, ESoundLevel::Level3);

	return 0.0f;
}

void ABossRobot::PlayDeadAnimation()
{
	AnimInst->Montage_Play(DeadMontage, 1.0f);
}

void ABossRobot::SetDead()
{
	ABossAIController* BossAIController = Cast<ABossAIController>(GetController());
	if (BossAIController)
	{
		BossAIController->StopAI();
	}


	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	//UI_Heart->SetHiddenInGame(true);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 3.0f, false);
}

void ABossRobot::PunchAttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 100.f;
	const float AttackRadius = Stat->GetTotalStat().AttackRadius;
	const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MONSTER, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		HasNextCombo = true;
	}
}



void ABossRobot::ProjectileCastEnd()
{
	OnCastFinished.ExecuteIfBound();

}

void ABossRobot::LandAttack()
{
	//카메라 쉐이크
	AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());
	TSubclassOf<UCameraShakeBase> CameraShakeClass = UCameraShakeBase::StaticClass();
	FPSPlayerController->PlayerCameraManager->StartCameraShake(UCharacterCameraShake::StaticClass(), 10.0f);

	AMonsterProjectile* NewBullet = GetWorld()->SpawnActor<AMonsterProjectile>(AMonsterProjectile::StaticClass(), GetActorLocation(), GetActorRotation());
	
	if (NewBullet)
	{
		NewBullet->GetCollisionComp()->InitSphereRadius(200.f);
		NewBullet->SetBulletInfo(0.f, 50.f, 3.0f);

		//DrawDebugCapsule(GetWorld(), NewBullet->GetActorLocation(), 200.f, 200.f, FRotationMatrix::MakeFromZ(NewBullet->GetActorForwardVector()).ToQuat(), FColor::Yellow, false, 5.0f);

	}


	//Cast 파티클
	AParticleNiagara* CastParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), GetActorLocation(), GetActorRotation());
	CastParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_prazmaShock.NS_ky_prazmaShock'"));
	//NewBullet->SetRootComponent(NewBullet->GetRootComponent());

	CastParticle->GetParticleComponent()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	FVector AddLocation = GetActorLocation();
	AddLocation += {0, 0, -90};

	CastParticle->SetActorLocation(AddLocation);

	FVector NewScale = FVector(1.0f, 1.0f, 1.0f);
	CastParticle->GetParticleComponent()->SetRelativeScale3D(NewScale);
	CastParticle->SetParticleFinishedDestory();
	CastParticle->GetParticleComponent()->SetVariableFloat(FName("Velocity"), 0.3f);
}

void ABossRobot::ProjectileAttack()
{
	GetBossAnimInstance()->SetIsLaunch(true);


	AMonsterProjectile* NewBullet = GetWorld()->SpawnActor<AMonsterProjectile>(AMonsterProjectile::StaticClass(), GetActorLocation(), GetActorRotation());
	NewBullet->GetCollisionComp()->InitSphereRadius(100.f);
	NewBullet->SetBulletInfo(4000.f, 100.f, 3.0f);

	//Cast 파티클
	AParticleNiagara* BulletParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), GetActorLocation(), GetActorRotation());
	BulletParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_sonicExp.NS_ky_sonicExp'"));
	BulletParticle->GetParticleComponent()->AttachToComponent(NewBullet->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//FVector NewScale = FVector(3.0f, 3.0f, 3.0f);
	//BulletParticle->GetParticleComponent()->SetRelativeScale3D(NewScale);
	BulletParticle->GetParticleComponent()->SetVariableFloat(FName("Velocity"), 0.3f);

	NewBullet->OnDestory.AddUObject(BulletParticle, &AParticleNiagara::ParticleDestory);
}

void ABossRobot::RotationAttack(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 300.f * DeltaTime;
	SetActorRotation(CurrentRotation);
}

void ABossRobot::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		AnimInst->Montage_Play(ComboActionMontage);
		HasNextCombo = false;
		ComboActionBegin();
	}
}

void ABossRobot::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABossRobot::ComboActionEnd);
	AnimInst->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ABossRobot::ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	OnAttackFinished.ExecuteIfBound();

	//NotifyComboActionEnd();
}

void ABossRobot::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (HasNextCombo)
	{
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

		AnimInst->Montage_JumpToSection(NextSection, ComboActionMontage);
		FName Current = AnimInst->Montage_GetCurrentSection();

	//	ComboTimerHandle.Invalidate();
		SetComboCheckTimer();
		HasNextCombo = false;
	}
}

void ABossRobot::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f )
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ABossRobot::ComboCheck, ComboEffectiveTime, false);
	}
}

void ABossRobot::SetAICastDelegate(const FAIMonsterAttackFinished& InMonsterAttackFinished)
{
	OnCastFinished = InMonsterAttackFinished;
}

void ABossRobot::NotifyComboActionEnd()
{
	int a = 0;
}

void ABossRobot::TargetRush(FVector TargetLocation, float DeltaTime)
{
	//FVector Move = GetActorLocation();
	//Move.X += TargetLocation.X * 400.f * DeltaTime;
	//Move.Y += TargetLocation.Y * 400.f * DeltaTime;
	//Move.Z = 0.f;
	//SetActorLocation(Move);

	FVector LaunchVelocity = TargetLocation * 1500.f;
	LaunchCharacter(LaunchVelocity, true, true);
	//GetCharacterMovement()->MaxWalkSpeed = 700.f;
}

void ABossRobot::JumpAttack(FVector TargetLocation, EJumpAttackState State, float DeltaTime)
{
	AttackState = State;

	switch (AttackState)
	{
	case EJumpAttackState::Jump:
	{
		if (GetActorLocation().Z > 0.1f)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		}
		else
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		}

		GetCharacterMovement()->GravityScale = 0.f;
		GetCharacterMovement()->JumpZVelocity = 0.f;


		FVector LaunchVelocity = FVector::UpVector * 2500.f;
		LaunchCharacter(LaunchVelocity, true, true);
		//GetCharacterMovement()->MaxWalkSpeed = 300.f;

	}


		break;
	case EJumpAttackState::Land:
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

		GetCharacterMovement()->GravityScale = 10.f;
		GetCharacterMovement()->JumpZVelocity = 10.f;

		FVector LaunchVelocity = TargetLocation * 2500.f;
		LaunchCharacter(LaunchVelocity, true, true);
		//GetCharacterMovement()->MaxWalkSpeed = 300.f;

	}
		//AddMovementInput(TargetLocation , true, true);
	break;

	default:
		break;
	}
}

void ABossRobot::PunchAttack1(float DeltaTime)
{
	ProcessComboCommand();
}

void ABossRobot::PunchAttack2()
{
	AnimInst->SetIsPunchAttack2(true);

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 60.f;
	const float AttackRadius = 200.f;
	const float AttackDamage = Stat->GetTotalStat().Attack * 2;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MONSTER, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);

}
