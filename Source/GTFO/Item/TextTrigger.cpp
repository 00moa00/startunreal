// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TextTrigger.h"
#include "Components/BoxComponent.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/CharacterItemInterface.h"
#include "Physics/FPSCollision.h"
#include "UI/MainHUD.h"
#include "UI/GuideWidget.h"
#include "Character/FPSPlayerController.h"

// Sets default values
ATextTrigger::ATextTrigger()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATextTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ATextTrigger::BeginPlay()
{
	Super::BeginPlay();

	AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());

	UI_MainHUD = FPSPlayerController->GetMainHUD();
}

void ATextTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICharacterItemInterface* OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		if (UI_MainHUD == nullptr)
		{
			AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());
			UI_MainHUD = FPSPlayerController->GetMainHUD();
			UI_MainHUD->GetGuideWidget()->OnGuideTextChange(MainText, SubTextA, SubTextB);
			Destroy();
		}

	}
}