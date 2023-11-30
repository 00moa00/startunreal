// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BulletDataComponent.h"
#include "Data/GameDataSingleton.h"

// Sets default values for this component's properties
UBulletDataComponent::UBulletDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UBulletDataComponent::BeginPlay()
{
	Super::BeginPlay();
	BlueBullet = UGameDataSingleton::GetInst().GetBulletData("BlueBullet");
	RedBullet = UGameDataSingleton::GetInst().GetBulletData("RedBullet");
	GreenBullet = UGameDataSingleton::GetInst().GetBulletData("GreenBullet");
	// ...	
}


