// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterDataComponent.h"
#include "Data/GameDataSingleton.h"

// Sets default values for this component's properties
UMonsterDataComponent::UMonsterDataComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;

	//AttackRadius = 50.f;
}



void UMonsterDataComponent::SetMonsterType(FName _MonsterType)
{
	BaseStat = UGameDataSingleton::GetInst().GetMonsterData(_MonsterType);
	SetHp(BaseStat.MaxHp);
}


// Called when the game starts
void UMonsterDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UMonsterDataComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		//³ª Á×¾ú¿À
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UMonsterDataComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
}


