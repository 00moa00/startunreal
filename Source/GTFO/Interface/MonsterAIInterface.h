// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/Sound.h"

#include "MonsterAIInterface.generated.h"


UENUM()
enum class EStimulationLevel : uint8
{
	Sleep,
	Detect,
	Beware,
	Awake
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMonsterAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIMonsterAttackFinished);
DECLARE_DELEGATE(FAIUpdateDetectLevel);

class GTFO_API IMonsterAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) = 0;
	virtual void SetAIUpdateDetectLevelDelegate(const FAIUpdateDetectLevel& InUpdate) = 0;
	virtual void OnAIUpdateDetectLevel() = 0;

	virtual void SetStimulationLevel(EStimulationLevel _EStimulationLevel) = 0;
	virtual void ChangeStimulationLevel(ESoundLevel _Level) = 0;


	virtual EStimulationLevel GetStimulationLevel() = 0;

	virtual bool GetKnockBack() { return false; };
	//virtual void AttackByPlayer() = 0;
};
