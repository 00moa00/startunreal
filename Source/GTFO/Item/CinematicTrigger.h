// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovieSceneSequencePlayer.h"

#include "CinematicTrigger.generated.h"

UCLASS()
class GTFO_API ACinematicTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACinematicTrigger();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class ULevelSequence* CinematicLevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMovieSceneSequencePlaybackSettings CinematicSetting;

	class ULevelSequencePlayer* CinematicSequencePlayer;
	class ALevelSequenceActor* CinematicSequenceActor;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UFUNCTION()
	void SequenceFinish();
};
