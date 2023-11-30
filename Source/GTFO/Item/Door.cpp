// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Door.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/CharacterItemInterface.h"
#include "Physics/FPSCollision.h"
// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);

	Trigger->SetupAttachment(DoorMesh);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		bIsOverlap = true;
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingPawn = nullptr;
	bIsOverlap = false;
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpen == true && OpenTimer < 5.f)
	{
		OpenTimer += DeltaTime;
		MoveLocation = GetActorLocation();
		MoveLocation += OpenDirection * 50.f * DeltaTime;

		SetActorLocation(MoveLocation);
	}
	

	//플레이어 입력체크
	if (OverlappingPawn)
	{
		if (OverlappingPawn->GetIsInteractionAction() && bIsOverlap == true)
		{
			bIsOpen = true;
		}
	}

}


void ADoor::OpenDoor()
{
	bIsOpen = true;
}

bool ADoor::IsDoorOverlap()
{
	return bIsOverlap;
}
