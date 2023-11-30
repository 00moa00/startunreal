// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterStatComponent.h"
#include "Data/GameDataSingleton.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	//MaxHp = 200.f;
	//CurrentHp = MaxHp;
	CurrentLevel = 1;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	SetStemina(BaseStat.Stemina);
	MaxStemina = BaseStat.Stemina;

}

void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentStemina < MaxStemina)
	{
		float RecoveryStemina = CurrentStemina;
		RecoveryStemina += 1.0f * DeltaTime;
		CurrentStemina = FMath::Clamp<float>(RecoveryStemina, 0.0f, BaseStat.Stemina);
		OnSteminaChanged.Broadcast(CurrentStemina);
	}
}

void UCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UGameDataSingleton::GetInst().CharacterMaxLevel);
	BaseStat = UGameDataSingleton::GetInst().GetCharacterStatData(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);
}

float UCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	// FMath::Clamp -> 주어진 최소값과 최대갑 사이에 값을 고정시키는 역할
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);

	//KINDA_SMALL_NUMBER : 아주 작지만 실제 0은 아닌 값으로, 부동 소수점 연산에서 발생할 수 있는 반올림 오차와 같은 문제를 방지하는 데 도움을 준다.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		//나 죽었오
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

float UCharacterStatComponent::ApplyStemina(float InStemina)
{
	const float PrevStemina = CurrentStemina;
	// FMath::Clamp -> 주어진 최소값과 최대갑 사이에 값을 고정시키는 역할
	const float ActualStemina = FMath::Clamp<float>(InStemina, 0, InStemina);

	SetStemina(PrevStemina - ActualStemina);

	//KINDA_SMALL_NUMBER : 아주 작지만 실제 0은 아닌 값으로, 부동 소수점 연산에서 발생할 수 있는 반올림 오차와 같은 문제를 방지하는 데 도움을 준다.
	if (CurrentStemina <= KINDA_SMALL_NUMBER)
	{
		//나 죽었오
		//OnHpZero.Broadcast();
	}

	return ActualStemina;
}

void UCharacterStatComponent::RecoveryHp(float InKit)
{
	CurrentHp = FMath::Clamp<float>(CurrentHp + InKit, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);

}

void UCharacterStatComponent::AddBlueBullet(uint32 _Value)
{
	BlueBulllet += _Value;
	OnBlueBulletChanged.Broadcast(BlueBulllet);
}

void UCharacterStatComponent::AddGreenBullet(uint32 _Value)
{
	GreenBulllet += _Value;
	OnGreenBulletChanged.Broadcast(GreenBulllet);
}

void UCharacterStatComponent::AddRedBullet(uint32 _Value)
{
	RedBulllet += _Value;
	OnRedBulletChanged.Broadcast(RedBulllet);

}

bool UCharacterStatComponent::SubGreenBullet(uint32 _Value)
{
	const uint32 ActualBullet = FMath::Clamp<float>(_Value, 0, _Value);
	const uint32 PrevBulllet = GreenBulllet;
	GreenBulllet = FMath::Clamp<float>(PrevBulllet - ActualBullet, 0.0f, PrevBulllet - ActualBullet);

	OnGreenBulletChanged.Broadcast(GreenBulllet);

	if (GreenBulllet == 0)
	{
		return false;
	}

	return true;
}

bool UCharacterStatComponent::SubBlueBullet(uint32 _Value)
{
	const uint32 ActualBullet = FMath::Clamp<float>(_Value, 0, _Value);
	const uint32 PrevBulllet = BlueBulllet;
	BlueBulllet = FMath::Clamp<float>(PrevBulllet - ActualBullet, 0.0f, PrevBulllet - ActualBullet);

	OnBlueBulletChanged.Broadcast(BlueBulllet);

	if (BlueBulllet == 0)
	{
		return false;
	}

	return true;
}

bool UCharacterStatComponent::SubRedBullet(uint32 _Value)
{
	const uint32 ActualBullet = FMath::Clamp<float>(_Value, 0, _Value);
	const uint32 PrevBulllet = RedBulllet;
	RedBulllet = FMath::Clamp<float>(PrevBulllet - ActualBullet, 0.0f, PrevBulllet - ActualBullet);

	OnRedBulletChanged.Broadcast(RedBulllet);

	if (RedBulllet == 0)
	{
		return false;
	}

	return true;
}

void UCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	//나 변동됐오
	OnHpChanged.Broadcast(CurrentHp);
}

void UCharacterStatComponent::SetStemina(float NewStemina)
{
	CurrentStemina = FMath::Clamp<float>(NewStemina, 0.0f, BaseStat.Stemina);

	OnSteminaChanged.Broadcast(CurrentStemina);
}