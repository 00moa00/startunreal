// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_KnockBack.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include "Monster/MonsterBase.h"

UBTTask_KnockBack::UBTTask_KnockBack()
{
	NodeName = TEXT("KnockBack");

}

EBTNodeResult::Type UBTTask_KnockBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IMonsterAIInterface* AIPawn = Cast<IMonsterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterBase* Mosnter = Cast<AMonsterBase>(ControllingPawn);
	if (Mosnter)
	{
		FVector KnockbackDirection = TargetPawn->GetActorForwardVector(); // ³Ë¹é ¹æÇâ ¼³Á¤
		float KnockbackForce = 500.0f; // ³Ë¹é ¼¼±â ¼³Á¤

		// ³Ë¹é Àû¿ë
		FVector LaunchVelocity = KnockbackDirection * KnockbackForce;
		Mosnter->LaunchCharacter(LaunchVelocity, true, true);
	}


	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_DAMAGE, false);

	return EBTNodeResult::Succeeded;
}