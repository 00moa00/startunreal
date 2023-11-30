// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SoundDetect.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBTService_SoundDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SoundDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;


};
