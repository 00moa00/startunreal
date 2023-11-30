// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Target.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTService_Target : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Target();

protected:
	
	TObjectPtr<class APawn> PawnTarget;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
