// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAI/BTTask_TargetRush.h"
#include "Interface/MonsterAIInterface.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include  "Monster/BossRobot.h"
#include "Animation/FPSAnimInstance.h"

UBTTask_TargetRush::UBTTask_TargetRush()
{
	NodeName = TEXT("TargetRush");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_TargetRush::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	ElapsedTime = 0.f;
	FVector TargetLo = TargetPawn->GetActorLocation();
	FVector BossLoco = BossPawn->GetActorLocation();
	TargetLocation = (TargetLo - BossLoco).GetSafeNormal();

	BossPawn->GetBossAnimInstance()->SetIsRush(true);

	// 아직 시간이 남았으면 계속 실행
	return EBTNodeResult::InProgress;

}
void UBTTask_TargetRush::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += OwnerComp.GetWorld()->GetDeltaSeconds();

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	BossPawn->TargetRush(TargetLocation, GetWorld()->GetDeltaSeconds());

	if (ElapsedTime >= 2.0f)
	{
		// Task를 종료시킨다.
		ElapsedTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

}

void UBTTask_TargetRush::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	BossPawn->GetBossAnimInstance()->SetIsRush(false);
	//BossPawn->GetBossAnimInstance()->SetIsLand(true);

}

