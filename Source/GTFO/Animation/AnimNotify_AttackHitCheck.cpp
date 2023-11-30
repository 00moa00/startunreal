// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/AnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		//직접참조X 인터페이스로 바꾸기
		IAnimationAttackInterface* Robot = Cast<IAnimationAttackInterface>(MeshComp->GetOwner());
		if (Robot)
		{
			Robot->PunchAttackHitCheck();
		}
	}
}
