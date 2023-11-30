// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpectrumEffect.generated.h"

UENUM()
enum class ESpectrumMaker : uint8
{
	Wait,
	Make,
};

UCLASS()
class GTFO_API ASpectrumEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpectrumEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetPoseableMesh(class ACharacterBase* _Character);
	void Init(USkeletalMeshComponent* Pawn);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spectrum, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> SpectrumMeshCapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spectrum, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> SpectrumMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spectrum, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPoseableMeshComponent> PoseableMesh;	//메쉬의 포즈정보만 복사해오는 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spectrum, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterial> GhostMaterial;				
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spectrum, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInstance> GhostMaterialInst;

	TObjectPtr < class UMaterialInstanceDynamic> GhostDynamicMaterial;	//매테리얼은 한 메쉬당 한개이므로 여러개 생성
	TObjectPtr < class USkeletalMeshComponent> PawnComponent;
	ESpectrumMaker SpectrumMakerState;

	float SpectrumTimer = 0.f;
	uint32 SpectrumCount = 0;

	float	fAlphaCount;
	float	fQuarterAlpha;
	bool	bSpawned;
};
