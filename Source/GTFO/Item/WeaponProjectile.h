// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDestoryDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDestoryDelegateLocation, FVector /*Location*/, bool);


UENUM()
enum class EProjectileType : uint8
{
	Blue,
	Green,
	Red,
	ETC
};

UCLASS()
class GTFO_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AWeaponProjectile();

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<class USphereComponent> CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

protected:
	uint32 BulletDamage;
	bool isHit; //´ê¾Æ¼­ Á×Àº°Å¸é true

public:
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UFUNCTION()
	void SetMovementDirection(FVector _Direction);

	UFUNCTION()
	void SetShouldBounce(bool _Bounce);

	UFUNCTION()
	void SetBulletInfo(float _Speed, uint32 _Damage, float _LifeSpan);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetProjectileType(EProjectileType _Type);

public:
	FOnDestoryDelegate OnDestory;
	FOnDestoryDelegateLocation OnDestoryLocation;
	EProjectileType ProjectileType;
	//FOnDestoryDelegate On
};
