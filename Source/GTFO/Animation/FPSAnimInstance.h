// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UFPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFPSAnimInstance();

public:
	FORCEINLINE void SetMoveDir(FVector2D _MoveDir){ MoveDir = _MoveDir; }
	FORCEINLINE void SetIsDash(uint8 _IsDash) { bIsDash = _IsDash; }
	FORCEINLINE void SetIsFire(uint8 _IsFire) { bIsFire = _IsFire; }
	FORCEINLINE void SetIsKnockBack(uint8 _IsKnockBack) { bIsKnockBack = _IsKnockBack; }
	FORCEINLINE void SetIsDetect(uint8 _IsDetect) { bIsDetect = _IsDetect; }
	FORCEINLINE void SetIsBeware(uint8 _IsBeware) { bIsBeware = _IsBeware; }
	FORCEINLINE void SetIsRush(uint8 _IsRush) { bIsRush = _IsRush; }
	FORCEINLINE void SetIsFalling(uint8 _IsFalling) { bIsFalling = _IsFalling; }
	FORCEINLINE void SetIsJumping(uint8 _IsJumping) { bIsJumping = _IsJumping; }
	FORCEINLINE void SetIsLand(uint8 _IsLand) { bIsLand = _IsLand; }
	FORCEINLINE void SetIsLaunch(uint8 _IsLaunch) { bIsLaunch = _IsLaunch; }
	FORCEINLINE void SetIsIdle(uint8 _IsIdle) { bIsIdle = _IsIdle; }
	FORCEINLINE void SetIsHit(uint8 _IsIHit) { bIsHit = _IsIHit; }
	FORCEINLINE uint8 GetIsHit() {return bIsHit;}
	FORCEINLINE void SetIsPunchAttack2(uint8 _IsAttack) {bIsPunchAttack2 = _IsAttack;}

	FORCEINLINE void SetControllerRotation(FRotator _Value) { ControllerRatation = _Value; }

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould; // 행동하고 있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFire : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould; // 점프하고 있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector2D MoveDir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDash : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDeath : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FRotator ControllerRatation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsKnockBack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDetect : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsBeware : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRush: 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsLand : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsLaunch : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsHit: 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsPunchAttack2 : 1;
	//
};
