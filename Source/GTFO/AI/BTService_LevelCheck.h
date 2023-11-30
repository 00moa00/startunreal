// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AI/GTFOAI.h"
#include "Interface/MonsterAIInterface.h"

#include "BTService_LevelCheck.generated.h"


UENUM()
enum class EStimulationTargetOneState : uint8
{
	Wait,
	Check,
};


UCLASS()
class GTFO_API UBTService_LevelCheck : public UBTService
{
	GENERATED_BODY()

public :
	UBTService_LevelCheck();

private:
	EStimulationTargetOneState StimulationState;
	UBehaviorTreeComponent* OwnerComponent;
	IMonsterAIInterface* OwnerComponentInterface;
	float Wait;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

public:
	void SetValueStimulationLevel();
};
