// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PunchAttack2.generated.h"

UENUM()
enum class EPunchAttack2State : uint8
{
	AttackStart,
	AttackEnd
};


UCLASS()
class GTFO_API UBTTask_PunchAttack2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PunchAttack2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float ElapsedTime;
	EPunchAttack2State PunchAttackState;

};
