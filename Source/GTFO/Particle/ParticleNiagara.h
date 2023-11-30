// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particle/ParticleBase.h"
#include "ParticleNiagara.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnParticleDestoryDelegate);

UCLASS()
class GTFO_API AParticleNiagara : public AParticleBase
{
	GENERATED_BODY()
	
	AParticleNiagara();
	
	protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    TObjectPtr<class UNiagaraComponent> mParticleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    TObjectPtr<class UNiagaraSystem> mNiagaraSystem;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	//UE_LOG(LogCharacter, Log, TEXT("Shoot"));
    virtual void SetParticle(class UNiagaraSystem* Particle);
    virtual void SetParticle(const FString& Path);

public:
    UFUNCTION()
    void ParticleDestory(UNiagaraComponent* Particle);

   // UFUNCTION()
    void ParticleDestory();

    UFUNCTION()
    void SetParticleFinishedDestory();
    
    TObjectPtr<class UNiagaraComponent> GetParticleComponent() { return mParticleComponent; }

public:
    FOnParticleDestoryDelegate OnDestroy;
};
