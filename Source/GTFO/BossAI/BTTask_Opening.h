// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Opening.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTTask_Opening : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Opening();

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

};
