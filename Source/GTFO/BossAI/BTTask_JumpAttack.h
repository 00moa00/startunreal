// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Monster/BossRobot.h"

#include "BTTask_JumpAttack.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTTask_JumpAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_JumpAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

private:
	float ElapsedJumpTime;
	float ElapsedLandTime;

	TObjectPtr<class ABossRobot> BossPawn;
	TObjectPtr<class APawn> TargetPawn;

	EJumpAttackState AttackState;
	FVector TargetLocation;
	FVector TargetDirection;
};
