// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/BulletData.h"

#include "BulletDataComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GTFO_API UBulletDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletDataComponent();

	//FORCEINLINE void SetModifierStat(const FBulletData& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE FBulletData GetBlueBulletData() const { return BlueBullet; }
	FORCEINLINE FBulletData GetGreenBulletData() const { return GreenBullet; }
	FORCEINLINE FBulletData GetRedBulletData() const { return RedBullet; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FBulletData BlueBullet;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FBulletData GreenBullet;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FBulletData RedBullet;
};
