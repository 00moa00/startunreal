// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Physics/FPSCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/FPSWidgetComponent.h"
#include "UI/HeartWidget.h"
#include "Animation/FPSAnimInstance.h"
#include "Monster/MonsterDataComponent.h"
//#include "AI/MonsterAIController.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
	StimulationLevel = EStimulationLevel::Sleep;
 
	PrimaryActorTick.bCanEverTick = true;
	DetectRange = 400.f;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MONSTER);

	////Movement
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	//GetCharacterMovement()->AirControl = 0.04f;						//캐릭터가 공중에 있는 경우 점프나 낙하 중에 어떻게 조종되는지 제어하는 속성. 높을수록 공중에서 더 쉽게 움직일 수 있다.
	//GetCharacterMovement()->MaxWalkSpeed = 500.f;
	//GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	//GetCharacterMovement()->BrakingDecelerationWalking = 200.f;	//반대 방향으로 이동하려 했을 때 감속

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);

	// Stat Component 
	Stat = CreateDefaultSubobject<UMonsterDataComponent>(TEXT("Stat"));

	//animaion
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/GTFO/Animation/BP_FloatRobotAnimaion.BP_FloatRobotAnimaion_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}


}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	Stat->SetMonsterType("Robot");
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

float AMonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("MonsterBaseTakeDamage"));
	//DetectRange = 1200.f;

	return 0.0f;
}

void AMonsterBase::SetDead()
{

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	UI_Heart->SetHiddenInGame(true);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AMonsterBase::PlayDeadAnimation()
{

}

void AMonsterBase::SetupCharacterWidget(UMasterWidget* InUserWidget)
{
	UHeartWidget* HeartWidget = Cast<UHeartWidget>(InUserWidget);
	if (HeartWidget)
	{
		//HeartWidget->SetMaxHp(Stat->GetMaxHP());
		//HeartWidget->SetHp(Stat->GetCurrentHp());
		//Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
	}
}

void AMonsterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	//Stat->ApplyDamage();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("MonsterOnOverlapBegin"));
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Stat->GetTotalStat().Attack, DamageEvent, GetController(), this);
}

float AMonsterBase::GetAIPatrolRadius()
{
	return 500.0f;
}

float AMonsterBase::GetAIDetectRange()
{
	return DetectRange; 
}

float AMonsterBase::GetAIAttackRange()
{
	return 50.0f;
}

float AMonsterBase::GetAITurnSpeed()
{
	return 50.f;
}

void AMonsterBase::SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;

}

void AMonsterBase::SetAIUpdateDetectLevelDelegate(const FAIUpdateDetectLevel& InUpdate)
{
	OnUpdate = InUpdate;
}

void AMonsterBase::OnAIUpdateDetectLevel()
{
	OnUpdate.ExecuteIfBound();
}


void  AMonsterBase::ChangeStimulationLevel(ESoundLevel _Level)
{

}

void AMonsterBase::NotifyAttackEnd()
{
	OnAttackFinished.ExecuteIfBound();
}
