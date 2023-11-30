// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Physics/FPSCollision.h"
#include "Particle/ParticleNiagara.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Character/CharacterBase.h"

// Sets default values
AMonsterProjectile::AMonsterProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->BodyInstance.SetCollisionProfileName("MonsterPreset");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsterProjectile::OnOverlapBegin);
	CollisionComp->OnComponentHit.AddDynamic(this, &AMonsterProjectile::OnHit);

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MonsterProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 3.0f;

	isHit = false;
}

void AMonsterProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AWeaponProjectile"));
	//FDamageEvent DamageEvent;
	//OtherActor->TakeDamage(BulletDamage, DamageEvent, nullptr, this);

	//DrawDebugCapsule(GetWorld(), SweepHitResult.Location, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (Character)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AWeaponProjectile"));
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this);
		isHit = true;
		Destroy();

		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
}

void AMonsterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
	if (Character)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AWeaponProjectile"));
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, nullptr, this);
		isHit = true;
		Destroy();

		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
}

void AMonsterProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		if (OnDestory.IsBound())
		{
			OnDestory.Broadcast();
		}

		// 액터가 파괴될 때 메시지를 출력
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AMonsterProjectile"));
	}
}

void AMonsterProjectile::SetBulletInfo(float _Speed, uint32 _Damage, float _LifeSpan)
{
	Damage = _Damage;

	ProjectileMovement->InitialSpeed = _Speed;
	ProjectileMovement->MaxSpeed = _Speed;

	InitialLifeSpan = _LifeSpan;
}

void AMonsterProjectile::SetShouldBounce(bool _Bounce)
{
	ProjectileMovement->bShouldBounce = _Bounce;
}
