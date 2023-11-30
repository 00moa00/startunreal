// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SoundDetect.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "Interface/MonsterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/FPSCollision.h"
#include "Monster/MonsterBase.h"

UBTService_SoundDetect::UBTService_SoundDetect()
{
}

void UBTService_SoundDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	//if (nullptr == ControllingPawn)
	//{
	//	return;
	//}
	//UWorld* World = ControllingPawn->GetWorld();
	//if (nullptr == World)
	//{
	//	return;
	//}
	//IMonsterAIInterface* AIPawn = Cast<IMonsterAIInterface>(ControllingPawn);
	//if (nullptr == AIPawn)
	//{
	//	return;
	//}


	//FVector Center = ControllingPawn->GetActorLocation();
	//float DetectRadius = 500.f;


	//TArray<FOverlapResult> OverlapResults;
	//FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	//bool bResult = World->OverlapMultiByChannel(
	//	OverlapResults,
	//	Center,
	//	FQuat::Identity,
	//	CCHANNEL_SOUND,
	//	FCollisionShape::MakeSphere(DetectRadius),
	//	CollisionQueryParam
	//);

	//if (bResult)
	//{
	//	for (auto const& OverlapResult : OverlapResults)
	//	{
	//		AMonsterBase* Pawn = Cast<AMonsterBase>(OverlapResult.GetActor());
	//		if (Pawn)
	//		{
	//			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Black, false, 1.f);
	//			DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Black, false, 1.f);
	//			return;
	//		}
	//	}
	//}
	//OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}

void UBTService_SoundDetect::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
}
