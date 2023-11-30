// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBox.h"
#include "Engine/AssetManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/CharacterItemInterface.h"
#include "Data/ItemData.h"
#include "Physics/FPSCollision.h"

// Sets default values
AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsOpen = false;

	Item = CreateDefaultSubobject<UItemData>(TEXT("Item"));

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxlidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxlidMesh"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

	RootComponent = Trigger;
	BoxMesh->SetupAttachment(Trigger);
	BoxlidMesh->SetupAttachment(Trigger);
	ItemMesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_TRIGGER);
	Trigger->SetBoxExtent(FVector(60.0f, 60.0f, 60.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();	
}

void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsOpen == true && BoxRatation < 100.f)
	{
		//ItemMesh->DestroyComponent();
		BoxRatation += 50.f * DeltaTime;
		BoxMesh->SetWorldRotation({ BoxRatation,BoxWorldRotation.Yaw,BoxWorldRotation.Roll });
	}

	if (BoxRatation > 100.f)
	{
		if (ItemMesh)
		{
			ItemMesh->DestroyComponent();

		}
	}

	//if()

}

void AItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ICharacterItemInterface* OverlappingPawn = Cast<ICharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
		if (BoxRatation < 100.f)
		{
			bIsOpen = true;
			BoxWorldRotation = GetActorRotation();
		}
	}
}

