// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "Character/OnTerminal.h"
#include "Character/CharacterBase.h"
#include "UI/MainHUD.h"

AFPSPlayerController::AFPSPlayerController()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainHUDRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/GTFO/UI/UI_Main.UI_Main_C'"));
	if (MainHUDRef.Succeeded())
	{
		MainHUDClass = MainHUDRef.Class;
	}

}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//게임 화면에서 플레이어의 입력을 처리하고 게임 외부의 다련 요소에 대한 입력을 무시한다.
	//게임 플레이 중에는 플레이어의 입력만 처리되며 게임 의외의 메뉴나 인게임 UI 등에 대한 입력은 무시된다. 

	//게임 중에 사용자 인터페이스와 상호 작용을 하거나 게임 플레이를진행할 때 주로 사용된다.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	if (IsValid(MainHUDClass))
	{
		MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(), MainHUDClass));
		if (IsValid(MainHUD))
		{
			MainHUD->AddToViewport();
			//MainHUD->GetPlayerHPWidget()->SetHp(50.f);
		}
	}


}

void AFPSPlayerController::PossessCharacterPawn()
{
	Possess(BaseCharacterPawn);

	ACharacterBase* CharacterPawn = Cast<ACharacterBase>(BaseCharacterPawn);
	CharacterPawn->SetCharacterInput();
}

void AFPSPlayerController::PossessTerminalPawn()
{

	Possess(OnTerminalPawn);

	AOnTerminal* TerminalPawn = Cast<AOnTerminal>(OnTerminalPawn);
	TerminalPawn->SetTerminalInput();
}
