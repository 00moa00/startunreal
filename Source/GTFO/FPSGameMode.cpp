// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "UI/HPWidget.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C'"));
	//if (DefaultPawnClassRef.Class)
	//{
	//	DefaultPawnClass = DefaultPawnClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/GTFO/Blueprint/BP_Character.BP_Character_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/GTFO/Blueprint/BP_FPSPlayerController.BP_FPSPlayerController_C'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
		
	}



}

//InitGame -> PostLogin -> BeginPlay 함수 순서로 호출이 된다.
void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();
}
//
//void AFPSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//}
//
//void AFPSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
//{
//}
//
//void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
//{
//}
//
//void AFPSGameMode::Tick(float DeltaTime)
//{
//}
// 