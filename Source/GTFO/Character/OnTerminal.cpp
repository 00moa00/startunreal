// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OnTerminal.h"
#include "Camera/CameraComponent.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/SphereComponent.h"
#include "Data/TerminalItemDataList.h"

#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "FPSGameMode.h"

#include "Interface/TerminalDataInterface.h"

#include "UI/MainHUD.h"
#include "UI/TerminalWidget.h"
#include "Components/ScrollBox.h"
#include "UI/FPSWidgetComponent.h"
#include "UI/SightWidget.h"

// Sets default values
AOnTerminal::AOnTerminal()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");

	TerminalCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	TerminalCameraComponent->SetupAttachment(CollisionComp);
	TerminalCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	TerminalCameraComponent->bUsePawnControlRotation = true;

	//TerminalItemDataList = CreateDefaultSubobject<UTerminalItemDataList>(TEXT("TerminalItemDataList"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/GTFO/Input/IMC_Terminal.IMC_Terminal'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}


 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AOnTerminal::BeginPlay()
{
	Super::BeginPlay();

	// Create a CameraComponent	
	FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());
	FPSPlayerController->SetTerminalPawn(this);

	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			AActor* CurrentActor = *ActorItr;
			ITerminalDataInterface* SearchTerminalItem = Cast<ITerminalDataInterface>(CurrentActor);
			if (SearchTerminalItem)
			{
				if (SearchTerminalItem->GetTerminalItemData())
				{
					TerminalItemDataList.Add(SearchTerminalItem->GetTerminalItemData());
					TerminalItemObjectList.Add(SearchTerminalItem->GetTerminalItemData()->ID, SearchTerminalItem);
				}
			}
		}
	}
	if (TerminalItemDataList.Num() > 0)
	{
		for (int Key = 0; Key < TerminalItemDataList.Num() ; ++Key)
		{
			//중복으로 들어가는건 처리하면 좋을듯.

			AllCommand.Add(TerminalItemDataList[Key]->ID);
			AllCommand.Add(TerminalItemDataList[Key]->Name);
			AllCommand.Add(TerminalItemDataList[Key]->Info);
			AllCommand.Add(TerminalItemDataList[Key]->Location);
		}

		AllCommand.Add("list");
		AllCommand.Add("quarry");
		AllCommand.Add("Open");

	}

	//Main Hud
	GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UI_MainHUD = FPSPlayerController->GetMainHUD();
	if (UI_MainHUD)
	{
		FOnTerminalData InTerminalData;
		InTerminalData.AddLambda(
			[&](TArray<FString>& _InData)
			{
				SetTerminalData(_InData);
			}
		); 
		
		UI_MainHUD->GetTerminalText()->SetTerminalDataDelegate(InTerminalData);
		UI_MainHUD->GetTerminalText()->SetOwningActor(this);
	}
}

// Called every frame
void AOnTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOnTerminal::PossessedBy(AController* NewController)
{
	if (UI_MainHUD)
	{
		UI_MainHUD->GetTextScrollBox()->SetVisibility(ESlateVisibility::Visible);
		UI_MainHUD->GetSightWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called to bind functionality to input
void AOnTerminal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(TerminalExitAction, ETriggerEvent::Triggered, this, &AOnTerminal::ExitTerminal);
	}
}

void AOnTerminal::ExitTerminal()
{
	FPSPlayerController->PossessCharacterPawn();
}

void AOnTerminal::SetTerminalInput()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(FPSPlayerController->GetLocalPlayer()))
	{
		//기존 매핑 클리어
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

bool AOnTerminal::GetSearchCommand(FString _Command)
{
	if (AllCommand.Find(_Command) == -1)
	{
		return false;
	}
	else
	{
		return true;

	}
}

void AOnTerminal::SetTerminalData(TArray<FString>& _InData)
{
	TerminalData = _InData;

	if (TerminalData.IsEmpty())
	{
		UI_MainHUD->CreateEditableTextBox(FString("NoCommand"));
		CreateTerminalBox();
		return;
	}

	for (int SplitNum = 0; SplitNum < TerminalData.Num(); ++SplitNum)
	{
		if (!GetSearchCommand(TerminalData[SplitNum]))
		{
			UI_MainHUD->CreateEditableTextBox(FString("NoCommand"));
			CreateTerminalBox();
			return;
		}
	}


	//빈 공간
	UI_MainHUD->CreateEditableTextBox(FString(" "), FString(" "), FString(" "), FString(" "));

	//전체보여주기
	if (TerminalData[0] == "List" && TerminalData.Num() == 1)
	{
		//커맨드 분석하고(TerminalData) 나열
		UI_MainHUD->CreateEditableTextBox(FString("ID"), FString("NAME"), FString("INFO"), FString("LOCATION"));

		for (int Key = 0; Key < TerminalItemDataList.Num() ; ++Key)
		{
			UI_MainHUD->CreateEditableTextBox(TerminalItemDataList[Key]->ID, TerminalItemDataList[Key]->Name, TerminalItemDataList[Key]->Info, TerminalItemDataList[Key]->Location);
		}
	}

	//지정한 아이템만 나열하기 List_ ㅁㅁㅁ
	if (TerminalData[0] == "List" && TerminalData.Num() == 2)
	{
		//커맨드 분석하고(TerminalData) 나열
		UI_MainHUD->CreateEditableTextBox(FString("ID"), FString("NAME"), FString("INFO"), FString("LOCATION"));

		for (int Key = 0; Key < TerminalItemDataList.Num() ; ++Key)
		{
			if (_InData[1] == TerminalItemDataList[Key]->Name
				|| _InData[1] == TerminalItemDataList[Key]->Location
				|| _InData[1] == TerminalItemDataList[Key]->ID)
			{
				UI_MainHUD->CreateEditableTextBox(TerminalItemDataList[Key]->ID, TerminalItemDataList[Key]->Name, TerminalItemDataList[Key]->Info, TerminalItemDataList[Key]->Location);
			}
		}
	}

	//Quary_ㅁㅁㅁ 조종
	if (TerminalData[0] == "Quarry")
	{
		if (TerminalData[2] == "Open")
		{
			UI_MainHUD->CreateEditableTextBox(FString("OpenDoor"));

			TerminalItemObjectList[TerminalData[1]]->OpenDoor();
		}
		else
		{
			UI_MainHUD->CreateEditableTextBox(FString("NoCommand"));
			CreateTerminalBox();
		}
	}

	CreateTerminalBox();

}

void AOnTerminal::CreateTerminalBox()
{
	//빈 공간
	UI_MainHUD->CreateEditableTextBox(FString(" "), FString(" "), FString(" "), FString(" "));
	UI_MainHUD->CreateEditableTextBox(FString(" "), FString(" "), FString(" "), FString(" "));

	//Terminal 재생성
	UI_MainHUD->CreateTerminalBox();

	if (UI_MainHUD)
	{
		FOnTerminalData InTerminalData;
		InTerminalData.AddLambda(
			[&](TArray<FString>& _InData)
			{
				//FinishLatentTask : excute 테스크를 리턴한 다음에 이것이 마무리 되었다
				SetTerminalData(_InData);
			}
		);

		UI_MainHUD->GetTerminalText()->SetTerminalDataDelegate(InTerminalData);
		UI_MainHUD->GetTerminalText()->SetOwningActor(this);
	}
}
