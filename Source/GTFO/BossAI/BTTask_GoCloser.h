// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GoCloser.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTTask_GoCloser : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GoCloser();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	
	TObjectPtr<class ABossRobot> BossPawn;
	TObjectPtr<class APawn> TargetPawn;

	float ElapsedTime;
};
