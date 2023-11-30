// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_LevelCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"

UBTService_LevelCheck::UBTService_LevelCheck()
{
	StimulationState = EStimulationTargetOneState::Wait;
	NodeName = TEXT("LevelCheck");
	Interval = 1.0f;
}

void UBTService_LevelCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return;
	}
	ACharacterBase* Character = Cast<ACharacterBase>(Target);
	if (nullptr == Character)
	{
		return;
	}
}

void  UBTService_LevelCheck::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	OwnerComponent = &SearchData.OwnerComp;

	APawn* ControllingPawn = SearchData.OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	OwnerComponentInterface = Cast<IMonsterAIInterface>(ControllingPawn);
	if (nullptr == OwnerComponentInterface)
	{
		return;
	}

	SetValueStimulationLevel();

	//APawn* Target = Cast<APawn>(SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	//if (nullptr == Target)
	//{
	//	return;
	//}

	//SetValueStimulationLevel 머신변경용
	FAIUpdateDetectLevel SetUpdateDetectLevel;
	SetUpdateDetectLevel.BindLambda(
		[&]()
		{
			SetValueStimulationLevel();
		}
	);

	OwnerComponentInterface->SetAIUpdateDetectLevelDelegate(SetUpdateDetectLevel);
}


void  UBTService_LevelCheck::SetValueStimulationLevel()
{
	switch (OwnerComponentInterface->GetStimulationLevel())
	{
	case EStimulationLevel::Sleep:
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_SLEEP, true);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_DETECT, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_BEWARE, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_AWAKE, false);

		break;
	case EStimulationLevel::Detect:
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_SLEEP, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_DETECT, true);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_BEWARE, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_AWAKE, false);
		break;
	case EStimulationLevel::Beware:
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_SLEEP, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_DETECT, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_BEWARE, true);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_AWAKE, false);
		break;
	case EStimulationLevel::Awake:
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_SLEEP, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_DETECT, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_BEWARE, false);
		OwnerComponent->GetBlackboardComponent()->SetValueAsBool(BBKEY_AWAKE, true);
		break;
	default:
		break;
	}
}
