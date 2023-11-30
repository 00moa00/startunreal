// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_KnockBack.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTDecorator_KnockBack : public UBTDecorator
{
	GENERATED_BODY()
	
	
public:
	UBTDecorator_KnockBack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
