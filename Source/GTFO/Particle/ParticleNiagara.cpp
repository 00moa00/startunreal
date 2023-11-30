// Fill out your copyright notice in the Description page of Project Settings.


#include "Particle/ParticleNiagara.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AParticleNiagara::AParticleNiagara()
{
    mParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle"));

   // mParticle->SetupAttachment(mAudio);
}

void AParticleNiagara::BeginPlay()
{
    Super::BeginPlay();
}

void AParticleNiagara::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (OnDestroy.IsBound())
    {
        OnDestroy.Broadcast();
    }
}

void AParticleNiagara::SetParticle(UNiagaraSystem* Particle)
{


}

void AParticleNiagara::SetParticle(const FString& Path)
{
    mNiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, *Path);
    mParticleComponent->SetAsset(mNiagaraSystem);

    //UNiagaraFunctionLibrary::(NiagaraComponent, TEXT("InitialRotation"), FLinearColor(NewRotation.Roll, NewRotation.Pitch, NewRotation.Yaw));
   // mParticleComponent->Activate();
}

void AParticleNiagara::ParticleDestory(UNiagaraComponent* Particle)
{
    mParticleComponent->Deactivate();
    Destroy();
}

void AParticleNiagara::ParticleDestory()
{
    mParticleComponent->Deactivate();
    Destroy();
}
void AParticleNiagara::SetParticleFinishedDestory()
{
    mParticleComponent->OnSystemFinished.AddDynamic(this, &AParticleNiagara::ParticleDestory);
}
