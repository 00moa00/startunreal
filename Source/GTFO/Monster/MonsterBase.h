// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterWidgetInterface.h"
#include "Interface/MonsterAIInterface.h"
#include "Interface/TerminalDataInterface.h"

#include "MonsterBase.generated.h"



UCLASS()
class GTFO_API AMonsterBase : 
	public ACharacter, 
	public ICharacterWidgetInterface,
	public IMonsterAIInterface,
	public ITerminalDataInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Animation Section
//protected:
//	TObjectPtr<class UFPSAnimInstance> AnimInst;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	virtual void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;


	// UI Widget Section
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> UI_Heart;

	virtual void SetupCharacterWidget(class UMasterWidget* InUserWidget) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	// Stat Section
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMonsterDataComponent> Stat;

	UPROPERTY(EditAnywhere, Category = TerminalData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTerminalItemData> TerminalData;

	virtual TObjectPtr<UTerminalItemData> GetTerminalItemData() { return TerminalData; }

	// AI Section
public:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAIMonsterAttackFinished& InOnAttackFinished) override;
	virtual void SetAIUpdateDetectLevelDelegate(const FAIUpdateDetectLevel& InUpdate) override;

	void NotifyAttackEnd();

	FAIMonsterAttackFinished OnAttackFinished;

	FAIUpdateDetectLevel OnUpdate;
	EStimulationLevel StimulationLevel;

	float DetectRange;

public:
	virtual EStimulationLevel GetStimulationLevel() { return StimulationLevel; }
	virtual void SetStimulationLevel(EStimulationLevel _EStimulationLevel) { StimulationLevel = _EStimulationLevel; } 
	virtual void ChangeStimulationLevel(ESoundLevel _Level) override;
	virtual void OnAIUpdateDetectLevel() override;

	
	void SetDetectRange(float _DetectRange) { DetectRange = _DetectRange; }

	
};
