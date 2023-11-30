// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AFPSPlayerController();
	
protected:
	virtual void BeginPlay() override;

	TObjectPtr <class APawn> BaseCharacterPawn;
	TObjectPtr<class APawn> OnTerminalPawn;
	TObjectPtr<class UMainHUD> MainHUD;
	TSubclassOf <class UUserWidget> MainHUDClass;

public:
	void SetCharacterPawn(class APawn* _Pawn) { BaseCharacterPawn = _Pawn; }
	void SetTerminalPawn(class APawn* _Pawn) { OnTerminalPawn = _Pawn; }

	void PossessCharacterPawn();
	void PossessTerminalPawn();

	TObjectPtr<class UMainHUD> GetMainHUD() { return MainHUD; }

};
