// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound.generated.h"

UENUM()
enum class ESoundScale : uint8
{
	Scale0,
	Scale1,
	Scale2,
	Scale3,
};

UENUM()
enum class ESoundLevel : uint8
{
	Level1,
	Level2,
	Level3,
};


UCLASS()
class GTFO_API ASound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASound();

	TObjectPtr<class USphereComponent> GetCollisionComp() { return CollisionComp;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void SetSoundType(ESoundScale _Scale, ESoundLevel _Level);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<class USphereComponent> CollisionComp;

	ESoundLevel SoundLevel;
};
