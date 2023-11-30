// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_ProjectileAttack.h"
#include "Interface/MonsterAIInterface.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include  "Monster/BossRobot.h"
#include "Animation/FPSAnimInstance.h"

UBTTask_ProjectileAttack::UBTTask_ProjectileAttack()
{
	NodeName = TEXT("ProjectileAttack");
	bNotifyTick = true;
	ProjectileState = EProjectileState::Cast;
}

EBTNodeResult::Type UBTTask_ProjectileAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	BossPawn = Cast<ABossRobot>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossPawn)
	{
		return EBTNodeResult::Failed;
	}

	ProjectileState = EProjectileState::Cast;
	LaunchWaitTimer = 0.f;
	PrijectileTimer = 0.f;
	return EBTNodeResult::InProgress;

}

void UBTTask_ProjectileAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	switch (ProjectileState)
	{
	case EProjectileState::Cast:
		PrijectileTimer += DeltaSeconds;

		if (PrijectileTimer >= 1.0f)
		{
			PrijectileTimer = 0.f;
			BossPawn->GetBossAnimInstance()->SetIsLand(false);

			//BossPawn->GetBossAnimInstance()->SetIsLaunch(true);
			BossPawn->ProjectileAttack();
			ProjectileState = EProjectileState::Launch;
		}
		break;

	case EProjectileState::Launch:
		LaunchWaitTimer += DeltaSeconds;

		if (LaunchWaitTimer >= 1.0f)
		{
			LaunchWaitTimer = 0.f;
			BossPawn->GetBossAnimInstance()->SetIsLaunch(false);
			ProjectileState = EProjectileState::Cast;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}
		break;


	default:
		break;
	}
}
