// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "Interface/MonsterAIInterface.h"
#include "FloatRobot.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API AFloatRobot : public AMonsterBase
{
	GENERATED_BODY()
	
	AFloatRobot();
protected:
	virtual void BeginPlay() override;

public:
	TObjectPtr<class UFPSAnimInstance> AnimInst;

	float KnockBackTime;
	bool isKnockBack;

	//virtual void SetDead() override;
	virtual void PlayDeadAnimation() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ChangeStimulationLevel(ESoundLevel _Level) override;
	virtual void SetDead() override;

	virtual bool GetKnockBack() { return isKnockBack; }
};
