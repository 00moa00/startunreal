// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/WeaponProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Physics/FPSCollision.h"
#include "Particle/ParticleNiagara.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Monster/MonsterBase.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->BodyInstance.SetCollisionProfileName("WeaponPreset");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponProjectile::OnOverlapBegin);
	CollisionComp->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnHit);

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->MaxSpeed = 2500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	//ProjectileMovement->Velocity 
	InitialLifeSpan = 1.0f;

	ProjectileType = EProjectileType::ETC;

	isHit = false;
}


void AWeaponProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AWeaponProjectile"));
	//FDamageEvent DamageEvent;
	//OtherActor->TakeDamage(BulletDamage, DamageEvent, nullptr, this);

	//DrawDebugCapsule(GetWorld(), SweepHitResult.Location, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);

}

void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(OtherActor);
	if (Monster)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("AWeaponProjectile"));
		FDamageEvent DamageEvent;

		if (ProjectileType == EProjectileType::Green)
		{
			BulletDamage *= 10;
		}

		OtherActor->TakeDamage(BulletDamage, DamageEvent, nullptr, this);
		isHit = true;
		Destroy();
		
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
}

void AWeaponProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		//DrawDebugCapsule(GetWorld(), GetActorLocation(), CollisionComp->GetScaledSphereRadius(), CollisionComp->GetScaledSphereRadius(), FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);

		if (OnDestory.IsBound())
		{
			OnDestory.Broadcast();
		}
		if (OnDestoryLocation.IsBound())
		{
			OnDestoryLocation.Broadcast(GetActorLocation(), isHit);
		}

	}
}

void AWeaponProjectile::SetBulletInfo(float _Speed, uint32 _Damage, float _LifeSpan)
{
	BulletDamage = _Damage;

	ProjectileMovement->InitialSpeed = _Speed;
	ProjectileMovement->MaxSpeed = _Speed;

	InitialLifeSpan = _LifeSpan;
}

void AWeaponProjectile::SetMovementDirection(FVector _Direction)
{
	//ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->Velocity = _Direction.GetSafeNormal() *400.f;
}

void AWeaponProjectile::SetShouldBounce(bool _Bounce)
{
	ProjectileMovement->bShouldBounce = _Bounce;
}

void AWeaponProjectile::SetProjectileType(EProjectileType _Type)
{
	ProjectileType = _Type;
}