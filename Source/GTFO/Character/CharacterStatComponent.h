// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/CharacterStatData.h"
#include "CharacterStatComponent.generated.h"




DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSteminaChangedDelegate, float /*Stemina*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRedChangedDelegate, uint32 /*RedBullet*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGreenChangedDelegate, uint32 /*RedBullet*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBlueChangedDelegate, uint32 /*RedBullet*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GTFO_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;
	
public:
	FOnHpZeroDelegate OnHpZero; 
	FOnHpChangedDelegate OnHpChanged;
	FOnSteminaChangedDelegate OnSteminaChanged;

	FOnRedChangedDelegate OnRedBulletChanged;
	FOnBlueChangedDelegate OnBlueBulletChanged;
	FOnGreenChangedDelegate OnGreenBulletChanged;


	void SetLevelStat(int32 InNewLevel);
	float ApplyDamage(float InDamage);
	void RecoveryHp(float InKit);

	float ApplyStemina(float InStemina);

	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void SetModifierStat(const FCharacterStatData& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE FCharacterStatData GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	FORCEINLINE uint32 GetBlueBullet() const { return BlueBulllet; }
	FORCEINLINE uint32 GetGreenBullet() const { return GreenBulllet; }
	FORCEINLINE uint32 GetRedBullet() const { return RedBulllet; }
	FORCEINLINE float GetCurrentStemina() const { return CurrentStemina; }

	bool SubBlueBullet(uint32 _Value);
	bool SubGreenBullet(uint32 _Value);
	bool SubRedBullet(uint32 _Value);

	void AddBlueBullet(uint32 _Value);
	void AddGreenBullet(uint32 _Value);
	void AddRedBullet(uint32 _Value);

protected:
	void SetHp(float NewHp);
	void SetStemina(float NewStemina);

	//계속해서 바뀌는 값

	//Transient : 오브젝트를 저장할 때마다 속성들이 모두 다 디스크에 저장이 된다.
	//현재 hp값 같은 경우에는 게임을 할 때마다 새롭게 저장이 되기 대문에 저장할 필요가 없을 수도 있다.
	//디스크에 저장할 필요가없다면 트랜지먼트라는 키워트를 추가해서 
	//디스크로부터 저장할 대 불필요한 공간이 낭비되지 않도록 지정해줄 수가 있다.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentStemina;

	UPROPERTY()
	float MaxStemina;


	//가지고있는 총알 개수
	UPROPERTY(EditAnywhere, Category = Stat)
	uint32 BlueBulllet = 0;

	UPROPERTY(EditAnywhere, Category = Stat)
	uint32 GreenBulllet = 0;

	UPROPERTY(EditAnywhere, Category = Stat)
	uint32 RedBulllet = 0;


	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FCharacterStatData BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FCharacterStatData ModifierStat;

		
};
