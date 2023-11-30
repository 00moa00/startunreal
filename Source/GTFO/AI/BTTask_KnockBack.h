// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KnockBack.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTTask_KnockBack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_KnockBack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
