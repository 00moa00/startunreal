// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:


public:
	AFPSGameMode();

public:
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	//virtual void PostLogin(APlayerController* NewPlayer);
	//virtual void Tick(float DeltaTime) override;

public:

};
