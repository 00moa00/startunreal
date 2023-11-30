// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "Interface/AnimationAttackInterface.h"

#include "BossRobot.generated.h"

UENUM()
enum class EJumpAttackState : uint8
{
	Jump,
	Wait,
	Land,
	Attack
};

UCLASS()
class GTFO_API ABossRobot : public AMonsterBase, public IAnimationAttackInterface
{
	GENERATED_BODY()

public:
	ABossRobot();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

protected:
	TObjectPtr<class UFPSAnimInstance> AnimInst;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PlayDeadAnimation() override;
	virtual void SetDead() override;
	virtual void PunchAttackHitCheck() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RotationAttackMontage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBossComboActionData> ComboActionData;

	EJumpAttackState AttackState;

	FVector LandLocation;

public:
	TObjectPtr<class UFPSAnimInstance> GetBossAnimInstance() { return AnimInst; }

	void TargetRush(FVector TargetLocation, float DeltaTime);
	void JumpAttack(FVector TargetLocation, EJumpAttackState State, float DeltaTime);
	void PunchAttack1(float DeltaTime);
	void PunchAttack2();

	void ProjectileCastEnd();

	void LandAttack();
	void ProjectileAttack();
	void RotationAttack(float DeltaTime);
	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void ComboCheck();
	void NotifyComboActionEnd();
	void SetComboCheckTimer();

	int32 CurrentCombo = 0;
	bool HasNextCombo = false;
	FTimerHandle ComboTimerHandle;

	FAIMonsterAttackFinished OnCastFinished;
	void SetAICastDelegate(const FAIMonsterAttackFinished& InMonsterAttackFinished);


};
