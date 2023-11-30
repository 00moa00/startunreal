// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_KnockBack.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTService_KnockBack : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_KnockBack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
