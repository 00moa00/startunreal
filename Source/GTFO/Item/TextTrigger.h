// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextTrigger.generated.h"

UCLASS()
class GTFO_API ATextTrigger : public AActor
{
	GENERATED_BODY()
	
	public:
	// Sets default values for this actor's properties
	ATextTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	TObjectPtr<class UMainHUD> UI_MainHUD;

	UPROPERTY(EditAnywhere, Category = TextTriger)
	FString MainText = " ";

	UPROPERTY(EditAnywhere, Category = TextTriger)
	FString SubTextA = " ";

	UPROPERTY(EditAnywhere, Category = TextTriger)
	FString SubTextB = " ";

};
