// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CinematicTrigger.h"
#include "Components/BoxComponent.h"
#include "Physics/FPSCollision.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "UI/MainHUD.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Character/FPSPlayerController.h"

// Sets default values
ACinematicTrigger::ACinematicTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACinematicTrigger::OnOverlapBegin);

	CinematicSequencePlayer = nullptr;
	CinematicSequenceActor = nullptr;
}
void ACinematicTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (IsValid(CinematicLevelSequence))
	{
		if (!CinematicSequencePlayer)
		{
			CinematicSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(), CinematicLevelSequence, CinematicSetting, CinematicSequenceActor);
		}
		//AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());

		UMainHUD* UI_MainHUD = FPSPlayerController->GetMainHUD();
		UI_MainHUD->SetVisibility(ESlateVisibility::Hidden);
		/*
		재생이 시작될때
		FOnMovieSceneSequencePlayerEvent	OnPlay;

		역재생 될때
		FOnMovieSceneSequencePlayerEvent	OnPlayReverse;

		재생하던 시퀀스가 멈출때
		FOnMovieSceneSequencePlayerEvent	OnStop;

		재생하던 시퀀스가 일시정지 될때
		FOnMovieSceneSequencePlayerEvent	OnPause;

		재생하던 시퀀스가 끝날때
		FOnMovieSceneSequencePlayerEvent	OnFinished;
		*/

		CinematicSequencePlayer->Play();

		CinematicSequencePlayer->OnFinished.AddDynamic(this, &ACinematicTrigger::SequenceFinish);
	}
}

void ACinematicTrigger::SequenceFinish()
{
	AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());

	UMainHUD* UI_MainHUD = FPSPlayerController->GetMainHUD();
	UI_MainHUD->SetVisibility(ESlateVisibility::Visible);
	Destroy();
}
