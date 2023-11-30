// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_Opening.h"
#include "AI/GTFOAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Opening::UBTTask_Opening()
{
	NodeName = TEXT("Opening");

	bNotifyTaskFinished = true;
}

void UBTTask_Opening::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_OPENING, true);
}