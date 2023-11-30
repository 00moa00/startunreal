// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTService_Target.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/FPSCollision.h"
#include "Interface/MonsterAIInterface.h"
#include "DrawDebugHelpers.h"
#include "Character/CharacterBase.h"

UBTService_Target::UBTService_Target()
{
	NodeName = TEXT("Target");
	bNotifyOnSearch = true;
	//Interval = 1.0f;
}

 void UBTService_Target::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	 APawn* ControllingPawn = SearchData.OwnerComp.GetAIOwner()->GetPawn();
	 if (nullptr == ControllingPawn)
	 {
		 return;
	 }

	 FVector Center = ControllingPawn->GetActorLocation();
	 UWorld* World = ControllingPawn->GetWorld();

	 if (PawnTarget == nullptr)
	 {

		 TArray<FOverlapResult> OverlapResults;
		 FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
		 bool bResult = World->OverlapMultiByChannel(
			 OverlapResults,
			 Center,
			 FQuat::Identity,
			 ECC_Pawn,
			 FCollisionShape::MakeSphere(2000.f),
			 CollisionQueryParam
		 );

		 if (bResult)
		 {
			 for (auto const& OverlapResult : OverlapResults)
			 {
				 PawnTarget = Cast<APawn>(OverlapResult.GetActor());
				 if (PawnTarget && PawnTarget->GetController()->IsPlayerController())
				 {
					 SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, PawnTarget);

					 if (SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_STARTATTACK) == false)
					 {
						 SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_STARTATTACK, true);
						 SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, true);
					 }

					 //DrawDebugSphere(World, Center, 2000.f, 16, FColor::Green, false, 0.2f);

					 //DrawDebugPoint(World, PawnTarget->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
					 //DrawDebugLine(World, ControllingPawn->GetActorLocation(), PawnTarget->GetActorLocation(), FColor::Green, false, 0.27f);
					 return;
				 }
			 }
		 }
	 }
}


void UBTService_Target::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(1500.f),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ACharacterBase* DistanceCheck = Cast<ACharacterBase>(OverlapResult.GetActor());
			if (DistanceCheck && DistanceCheck->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDISTANCE, true);
				//DrawDebugSphere(World, Center, 1500.f, 16, FColor::Magenta, false, 0.2f);

				//DrawDebugPoint(World, DistanceCheck->GetActorLocation(), 10.0f, FColor::Magenta, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), DistanceCheck->GetActorLocation(), FColor::Magenta, false, 0.27f);
				return;
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDISTANCE, false);
			}
		}
	}



}