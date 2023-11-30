// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_PunchAttack2.h"
#include "Interface/MonsterAIInterface.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include  "Monster/BossRobot.h"
#include "Animation/FPSAnimInstance.h"


UBTTask_PunchAttack2::UBTTask_PunchAttack2()
{
	NodeName = TEXT("PunchAttack2");
	bNotifyTick = true;
	PunchAttackState = EPunchAttack2State::AttackStart;
}

EBTNodeResult::Type UBTTask_PunchAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	ABossRobot* BossPawn = Cast<ABossRobot>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}


	return EBTNodeResult::InProgress;

 }
void UBTTask_PunchAttack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	ABossRobot* BossPawn = Cast<ABossRobot>(OwnerComp.GetAIOwner()->GetPawn());
	//BossPawn->RotationAttack(DeltaSeconds);

	switch (PunchAttackState)
	{
	case EPunchAttack2State::AttackStart:
		ElapsedTime = 0.f;

		BossPawn->PunchAttack2();
		BossPawn->GetBossAnimInstance()->SetIsPunchAttack2(true);
		PunchAttackState = EPunchAttack2State::AttackEnd;
		break;
	case EPunchAttack2State::AttackEnd:
		ElapsedTime += DeltaSeconds;

		if (ElapsedTime > 2.0f)
		{
			BossPawn->GetBossAnimInstance()->SetIsPunchAttack2(false);

			PunchAttackState = EPunchAttack2State::AttackStart;
			ElapsedTime = 0.f;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}


		break;
	default:
		break;
	}



 }