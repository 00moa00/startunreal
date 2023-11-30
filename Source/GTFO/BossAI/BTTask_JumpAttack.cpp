// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAI/BTTask_JumpAttack.h"
#include "Interface/MonsterAIInterface.h"
#include "AI/GTFOAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MonsterAIInterface.h"
#include "Animation/FPSAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_JumpAttack::UBTTask_JumpAttack()
{
	NodeName = TEXT("JumpAttack");
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_JumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ElapsedJumpTime = 0.f;
	ElapsedLandTime = 0.f;
	AttackState = EJumpAttackState::Jump;

	BossPawn = Cast<ABossRobot>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossPawn)
	{
		return EBTNodeResult::Failed;
	}

	TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}
	BossPawn->GetBossAnimInstance()->SetIsJumping(true);
	BossPawn->GetBossAnimInstance()->SetIsFalling(false);
	BossPawn->GetBossAnimInstance()->SetIsLand(false);

	return EBTNodeResult::InProgress;

}
void UBTTask_JumpAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedJumpTime += DeltaSeconds;

	switch (AttackState)
	{
	case EJumpAttackState::Jump:
		BossPawn->JumpAttack(TargetPawn->GetActorLocation(), EJumpAttackState::Jump, DeltaSeconds);
		if (ElapsedJumpTime > 1.0f)
		{
			BossPawn->GetBossAnimInstance()->SetIsJumping(false);
			BossPawn->GetBossAnimInstance()->SetIsFalling(true);
			TargetLocation = TargetPawn->GetActorLocation();
			ElapsedJumpTime = 0.f;

			AttackState = EJumpAttackState::Wait;
		}
		break;

	case EJumpAttackState::Wait:
		if (ElapsedJumpTime > 0.5f)
		{
			ElapsedJumpTime = 0.f;
			FVector BossLoco = BossPawn->GetActorLocation();
			TargetDirection = (TargetLocation - BossLoco).GetSafeNormal();
			AttackState = EJumpAttackState::Land;
		}
		break;

	case EJumpAttackState::Land:

		{
			BossPawn->JumpAttack(TargetDirection, EJumpAttackState::Land, DeltaSeconds);

			FHitResult HitResult;
			FVector Start = BossPawn->GetActorLocation();
			FVector End = Start - FVector(0, 0, 100); 

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(BossPawn);
			CollisionParams.AddIgnoredActor(TargetPawn);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, CollisionParams) || BossPawn->GetCharacterMovement()->IsMovingOnGround())
			{
				FVector NewLocation = HitResult.ImpactPoint;
				//BossPawn->SetActorLocation(NewLocation);
				BossPawn->GetBossAnimInstance()->SetIsFalling(false);
				BossPawn->GetBossAnimInstance()->SetIsLand(true);


				AttackState = EJumpAttackState::Attack;
			}
		}

		break;

		case EJumpAttackState::Attack:
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_JUMP, false);

			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROJECTILE, true);
			BossPawn->LandAttack();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			break;
	default:
		break;
	}

	//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	


	//if (ElapsedJumpTime > 2.0f)
	//{
	//	BossPawn->JumpAttack(TargetPawn->GetActorLocation(), EJumpAttackState::Land, DeltaSeconds);
	//}
}

