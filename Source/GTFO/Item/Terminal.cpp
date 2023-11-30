// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Terminal.h"
#include "Components/BoxComponent.h"
#include "Interface/CharacterItemInterface.h"
#include "Character/FPSPlayerController.h"
#include "Character/CharacterBase.h"
#include "Physics/FPSCollision.h"

// Sets default values
ATerminal::ATerminal()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATerminal::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATerminal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATerminal::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICharacterItemInterface* OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		ACharacterBase* CastCharacter = Cast<ACharacterBase>(OtherActor);
		if (CastCharacter)
		{
			AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(CastCharacter->GetController());
			FPSPlayerController->PossessTerminalPawn();
		}
	}
}