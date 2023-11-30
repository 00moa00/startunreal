// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/MonsterData.h"
#include "MonsterDataComponent.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GTFO_API UMonsterDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterDataComponent();
	void SetMonsterType(FName _MonsterType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	FOnHpZeroDelegate OnHpZero;

	// Called every frame
	float ApplyDamage(float InDamage);


	FORCEINLINE void SetModifierStat(const FMonsterData& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE FMonsterData GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	//FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	//UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	//float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMonsterData BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FMonsterData ModifierStat;
};
