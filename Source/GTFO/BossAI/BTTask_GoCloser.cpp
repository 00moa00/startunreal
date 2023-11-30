// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_GoCloser.h"
#include "Interface/MonsterAIInterface.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include "Animation/FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/BossRobot.h"


UBTTask_GoCloser::UBTTask_GoCloser()
{
	NodeName = TEXT("GoCloser");
	bNotifyTick = true;
	ElapsedTime = 0.f;

}

EBTNodeResult::Type UBTTask_GoCloser::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	BossPawn = Cast<ABossRobot>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossPawn)
	{
		return EBTNodeResult::Failed;
	}

	TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}
void UBTTask_GoCloser::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;

	FVector Distance = TargetPawn->GetActorLocation() - BossPawn->GetActorLocation();
	FVector Direction = Distance.GetSafeNormal();
	FVector LaunchVelocity = Direction * 500.f;


	BossPawn->GetBossAnimInstance()->SetMoveDir({ 1,-1 });
	BossPawn->LaunchCharacter(LaunchVelocity, true, true);

	if (ElapsedTime > 2.0f || Distance.Length() < 100.f)
	{
		ElapsedTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	}

	//BossPawn->GetCharacterMovement()->MaxWalkSpeed = 700.f;
}