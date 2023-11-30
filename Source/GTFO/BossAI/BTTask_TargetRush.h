// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetRush.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTTask_TargetRush : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_TargetRush();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	float ElapsedTime;
	FVector TargetLocation;

	TObjectPtr<class ABossRobot> BossPawn;
};
