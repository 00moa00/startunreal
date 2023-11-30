// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RotationAttack.h"
#include "Monster/BossRobot.h"

void UAnimNotify_RotationAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ABossRobot* Robot = Cast<ABossRobot>(MeshComp->GetOwner());
		if (Robot)
		{
			Robot->ProjectileAttack();
		}
	}
}
