// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_KnockBack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/GTFOAI.h"
#include "Interface/MonsterAIInterface.h"

UBTDecorator_KnockBack::UBTDecorator_KnockBack()
{
	NodeName = TEXT("KnockBack");
}

bool UBTDecorator_KnockBack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IMonsterAIInterface* AIPawn = Cast<IMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	if (AIPawn->GetKnockBack() == true)
	{
		return true;
	}
	else
	{
		return false;
	}


	return false;
}