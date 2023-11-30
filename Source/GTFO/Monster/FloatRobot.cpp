// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/FloatRobot.h"
#include "Components/CapsuleComponent.h"
#include "Physics/FPSCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/FPSWidgetComponent.h"
#include "UI/HeartWidget.h"
#include "Animation/FPSAnimInstance.h"
#include "Monster/MonsterDataComponent.h"
#include "AI/MonsterAIController.h"
#include "Character/Sound.h"
#include "Components/SphereComponent.h"

AFloatRobot::AFloatRobot()
{
	isKnockBack = false;

	KnockBackTime = 0.5f;

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; //PlacedInWorldOrSpawned 지정해서 배치된 폰 혹은 스폰 액터를 통해 등장시킨 폰 모두 ai 컨트롤러에 통제받는다
	
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MONSTER);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFloatRobot::OnOverlapBegin);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GTFO/Animation/AM_FloatRobotDead.AM_FloatRobotDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void AFloatRobot::BeginPlay()
{
	Super::BeginPlay();
	Stat->SetMonsterType("Robot");
	Stat->OnHpZero.AddUObject(this, &AFloatRobot::SetDead);
	AnimInst = Cast<UFPSAnimInstance>(GetMesh()->GetAnimInstance());

}

void AFloatRobot::PlayDeadAnimation()
{
	//AnimInst->StopAllMontages(0.0f);
	AnimInst->Montage_Play(DeadMontage, 1.0f);
}


void AFloatRobot::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepHitResult);
}


float AFloatRobot::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	DetectRange = 2000.f;

	StimulationLevel = EStimulationLevel::Awake;
	OnAIUpdateDetectLevel();

	ASound* AMonsterSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
	AMonsterSound->SetSoundType(ESoundScale::Scale3, ESoundLevel::Level3);

	isKnockBack = true;
	AnimInst->SetIsKnockBack(true);

	FTimerHandle KnockBackHandle;
	GetWorld()->GetTimerManager().SetTimer(KnockBackHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			AnimInst->SetIsKnockBack(false);
			GetCharacterMovement()->MaxWalkSpeed = 200.f;

			isKnockBack = false;
			//Destroy();
		}
	), KnockBackTime, false);


	return 0.0f;
}

void AFloatRobot::SetDead()
{
	AMonsterAIController* MonsterAIController = Cast<AMonsterAIController>(GetController());
	if (MonsterAIController)
	{
		MonsterAIController->StopAI();
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
	), DeadEventDelayTime, false);
}

void AFloatRobot::ChangeStimulationLevel(ESoundLevel _Level)
{

	if (_Level == ESoundLevel::Level3)
	{
		AnimInst->SetIsBeware(false);
		AnimInst->SetIsDetect(false);
		AnimInst->SetIsIdle(false);

		StimulationLevel = EStimulationLevel::Awake;
		return;
	}

	//if (_Level == ESoundLevel::Level1 && StimulationLevel == EStimulationLevel::Detect)
	//{
	//	return;
	//}

	switch (StimulationLevel)
	{
	case EStimulationLevel::Sleep:
		StimulationLevel = EStimulationLevel::Detect;
		if (_Level == ESoundLevel::Level2)
		{
			StimulationLevel = EStimulationLevel::Beware;
			AnimInst->SetIsBeware(true);

			break;
		}
		AnimInst->SetIsDetect(true);
		break;
	case EStimulationLevel::Detect:
		AnimInst->SetIsBeware(true);
		StimulationLevel = EStimulationLevel::Beware;
		if (_Level == ESoundLevel::Level2)
		{
			StimulationLevel = EStimulationLevel::Awake;
			break;
		}

		break;
	case EStimulationLevel::Beware:


		StimulationLevel = EStimulationLevel::Awake;

		break;
	default:
		break;
	}

	if (StimulationLevel == EStimulationLevel::Awake)
	{
		//ASound* ADashSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
		//ADashSound->SetSoundType(ESoundScale::Scale2, ESoundLevel::Level3);

		AnimInst->SetIsBeware(false);
		AnimInst->SetIsDetect(false);
		AnimInst->SetIsIdle(false);
	}

	OnAttackFinished.ExecuteIfBound();


}