// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_KnockBack.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/FPSCollision.h"
#include "Interface/MonsterAIInterface.h"
#include "DrawDebugHelpers.h"

UBTService_KnockBack::UBTService_KnockBack()
{

}

void UBTService_KnockBack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return ;
	}

	IMonsterAIInterface* AIPawn = Cast<IMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return ;
	}

	if (AIPawn->GetKnockBack() == true)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_DAMAGE, true);

	}


}