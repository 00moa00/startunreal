// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ProjectileAttack.generated.h"

UENUM()
enum class EProjectileState : uint8
{
	Cast,
	CastEndWait,
	LaunchWait,
	Launch,
	Rotation
};

UCLASS()
class GTFO_API UBTTask_ProjectileAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ProjectileAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	EProjectileState ProjectileState;

	TObjectPtr<class ABossRobot> BossPawn;
	float PrijectileTimer;
	float LaunchWaitTimer;
};
