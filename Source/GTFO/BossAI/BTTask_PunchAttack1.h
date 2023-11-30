// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PunchAttack1.generated.h"

UENUM()
enum class EPunchAttack1State : uint8
{
	AttackStart,
	AttackEnd
};

UCLASS()
class GTFO_API UBTTask_PunchAttack1 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PunchAttack1();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	EPunchAttack1State PunchAttackState;

};
