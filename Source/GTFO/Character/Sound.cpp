// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Sound.h"
#include "Components/SphereComponent.h"
#include "Monster/FloatRobot.h"
#include "Monster/MonsterBase.h"

// Sets default values
ASound::ASound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Super::BeginPlay();
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ASoundComp"));
	CollisionComp->SetSphereRadius(5.0f);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->BodyInstance.SetCollisionProfileName("SoundPreset");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASound::OnOverlapBegin);
	RootComponent = CollisionComp;

	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void ASound::BeginPlay()
{


}

// Called every frame
void ASound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ASound::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(SweepHitResult.GetActor());
	if (Monster)
	{	
		//Monster->SetStimulationLevel(EStimulationLevel::Awake);
		Monster->SetDetectRange(2000.f);
		Monster->ChangeStimulationLevel(SoundLevel);
		Monster->OnAIUpdateDetectLevel();
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AWeaponProjectile"));
	//FDamageEvent DamageEvent;
	//OtherActor->TakeDamage(BulletDamage, DamageEvent, nullptr, this);

	//DrawDebugCapsule(GetWorld(), SweepHitResult.Location, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);

}

void ASound::SetSoundType(ESoundScale _Scale, ESoundLevel _Level)
{
	SoundLevel = _Level;

	switch (_Scale)
	{
	case ESoundScale::Scale0:
		CollisionComp->SetSphereRadius(0.f);
		break;
	case ESoundScale::Scale1:
		CollisionComp->SetSphereRadius(500.f);
		break;
	case ESoundScale::Scale2:
		CollisionComp->SetSphereRadius(1000.f);
		break;
	case ESoundScale::Scale3:
		CollisionComp->SetSphereRadius(2000.f);
		break;
	default:
		break;
	}
}
