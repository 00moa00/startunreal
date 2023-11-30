// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_RandomPattern.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "AI/GTFOAI.h"

UBTTask_RandomPattern::UBTTask_RandomPattern()
{
	NodeName = TEXT("RandomPatternTask");
	BossPattern = EBossPattern::RUSH;
}

EBTNodeResult::Type UBTTask_RandomPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	RandomPattern(OwnerComp);



	return EBTNodeResult::Succeeded;
}


void UBTTask_RandomPattern::RandomPattern(UBehaviorTreeComponent& OwnerComp)
{
	bool IsDistance = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISDISTANCE);
	if (!IsDistance)
	{
		uint8 MinEnum = static_cast<uint8>(EBossPattern::NOT_DISTANCE_MIN) + 1;
		uint8 MaxEnum = static_cast<uint8>(EBossPattern::NOT_DISTANCE_MAX);
		uint8 RandomInRange = FMath::FRandRange(MinEnum, MaxEnum);
		BossPattern = static_cast<EBossPattern>(RandomInRange);

		if (PrevBossParrern == BossPattern)
		{
			RandomPattern(OwnerComp);
		}
		PrevBossParrern = BossPattern;

		switch (BossPattern)
		{
		case EBossPattern::JUMP1:
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RUSH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH2, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, false);

			break;
		case EBossPattern::RUSH:

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RUSH, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH2, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, false);

			break;
		default:
			break;
		}
	}

	else
	{
		uint8 MinEnum = static_cast<uint8>(EBossPattern::IN_DISTANCE_MIN) + 1;
		uint8 MaxEnum = static_cast<uint8>(EBossPattern::IN_DISTANCE_MAX);
		uint8 RandomInRange = FMath::FRandRange(MinEnum, MaxEnum);
		BossPattern = static_cast<EBossPattern>(RandomInRange);
		//BossPattern = EBossPattern::PUNCH2;

		if (PrevBossParrern == BossPattern)
		{
			RandomPattern(OwnerComp);
		}

		PrevBossParrern = BossPattern;

		switch (BossPattern)
		{
		case EBossPattern::JUMP2:
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RUSH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH2, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, false);

			break;

			case EBossPattern::PUNCH2:

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RUSH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH2, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, false);

			break;
		case EBossPattern::PUNCH:

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RUSH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH2, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, false);

			break;

		case EBossPattern::PROJECTILE:

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RUSH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH2, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PUNCH, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, true);


			break;

		default:
			break;
		}
	}
}
