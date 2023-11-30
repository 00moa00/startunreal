// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_PunchAttack1.h"
#include "Interface/MonsterAIInterface.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include  "Monster/BossRobot.h"

UBTTask_PunchAttack1::UBTTask_PunchAttack1()
{
	NodeName = TEXT("PunchAttack1");
	bNotifyTick = true;
	PunchAttackState = EPunchAttack1State::AttackStart;

}

EBTNodeResult::Type UBTTask_PunchAttack1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	PunchAttackState = EPunchAttack1State::AttackStart;

	FAIMonsterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			PunchAttackState = EPunchAttack1State::AttackEnd;
			//FinishLatentTask : excute 테스크를 리턴한 다음에 이것이 마무리 되었다
			//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	BossPawn->SetAIAttackDelegate(OnAttackFinished);

	// 아직 시간이 남았으면 계속 실행
	return EBTNodeResult::InProgress;

}
void UBTTask_PunchAttack1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	ABossRobot* BossPawn = Cast<ABossRobot>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr != BossPawn)
	{

		switch (PunchAttackState)
		{
		case EPunchAttack1State::AttackStart:
			BossPawn->PunchAttack1(DeltaSeconds);
			//PunchAttackState = EPunchAttack1State::AttackEnd;

			break;
		case EPunchAttack1State::AttackEnd:
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
			break;
		default:
			break;
		}
		//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	}

	//if (ElapsedTime >= 3.0f)
	//{
	//	// Task를 종료시킨다.
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	return;
	//}
}

