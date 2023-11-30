// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OnTerminal.generated.h"



UCLASS()
class GTFO_API AOnTerminal :
	public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOnTerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;


protected:
	TObjectPtr<class AFPSPlayerController> FPSPlayerController;

	UPROPERTY(VisibleDefaultsOnly, Category = OnTerminal)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> TerminalCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TerminalExitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class AFPSGameMode> GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UMainHUD> UI_MainHUD;

	TArray< TObjectPtr<class UTerminalItemData>> TerminalItemDataList;
	TMap<FString, TObjectPtr<class ITerminalDataInterface>> TerminalItemObjectList;
	TArray<FString> AllCommand;

	void ExitTerminal();

public:
	void SetTerminalInput();

public:
	//커맨드 분석용
	TArray<FString> TerminalData;

	TArray< TObjectPtr<class UTerminalItemData>> GetTerminalItemDataList() { return TerminalItemDataList; }
	void SetTerminalData(TArray<FString>& _InData);

	bool GetSearchCommand(FString _Command);
	void CreateTerminalBox();
};
