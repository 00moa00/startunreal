// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomPattern.generated.h"

UENUM()
enum class EBossPattern : uint8
{
	NOT_DISTANCE_MIN,
	JUMP1,
	RUSH,
	NOT_DISTANCE_MAX,

	IN_DISTANCE_MIN,
	JUMP2,
	PUNCH2,
	PUNCH,
	PROJECTILE,
	IN_DISTANCE_MAX
};


UCLASS()
class GTFO_API UBTTask_RandomPattern : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_RandomPattern();
	void RandomPattern(UBehaviorTreeComponent& OwnerComp);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	EBossPattern BossPattern;
	EBossPattern PrevBossParrern;
};
