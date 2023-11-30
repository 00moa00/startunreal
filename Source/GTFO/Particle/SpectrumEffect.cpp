// Fill out your copyright notice in the Description page of Project Settings.


#include "Particle/SpectrumEffect.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Character/CharacterBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"

// Sets default values
ASpectrumEffect::ASpectrumEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpectrumMeshCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>((TEXT("SpectrumEffectCollision")));
	SpectrumMeshCapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = SpectrumMeshCapsuleComponent;

	bSpawned = false;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("POSEABLEMESH"));
	RootComponent = PoseableMesh;

	//복사할 대상의 스켈레탈 메시를 불러온다
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PoseMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'"));
	if (SK_PoseMesh.Succeeded())
	{
		PoseableMesh->SetSkeletalMesh(SK_PoseMesh.Object);
	}
	//바꿔줄 메테리얼 효과를 불러온다.
	ConstructorHelpers::FObjectFinder<UMaterial> M_GhostTail(TEXT("/Script/Engine.Material'/Game/GTFO/Matertial/M_Outline_3.M_Outline_3'"));
	if (M_GhostTail.Succeeded())
	{
		GhostMaterial = M_GhostTail.Object;
	}

	//바꿔줄 메테리얼 효과를 불러온다.
	ConstructorHelpers::FObjectFinder<UMaterialInstance> M_GhostTailInst(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/GTFO/Matertial/M_Outline_Inst_2.M_Outline_Inst_2'"));
	if (M_GhostTailInst.Succeeded())
	{
		GhostMaterialInst = M_GhostTailInst.Object;
	}
}

// Called when the game starts or when spawned
void ASpectrumEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpectrumEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawned == true)
	{
		fAlphaCount -= 0.01f;


		for (int32 i = 0; i < PawnComponent->SkeletalMesh->Materials.Num(); i++)
		{
			GhostDynamicMaterial->SetScalarParameterValue("Opacity", fAlphaCount );
			PoseableMesh->SetMaterial(i, GhostDynamicMaterial);
		}

		if (fAlphaCount < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("GhostDestroy"));
			Destroy();
		}
	}
}

void ASpectrumEffect::Init(USkeletalMeshComponent* Pawn)
{
	//생성을 요청한 객체의 SkeletalMesh 포즈를 복제한다.
	PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);
	PawnComponent = Pawn;

	GhostDynamicMaterial = UMaterialInstanceDynamic::Create(GhostMaterialInst, this);

	for (int32 i = 0; i < Pawn->SkeletalMesh->Materials.Num(); i++)
		PoseableMesh->SetMaterial(i, GhostDynamicMaterial);

	fQuarterAlpha = fAlphaCount = 0.7f;
	bSpawned = true;
	SetActorLocation(GetActorLocation() + FVector(0, 10, -90));
	SetActorRotation(GetActorRotation() + FRotator(0, -90, 0));
}

void ASpectrumEffect::SetPoseableMesh(class ACharacterBase* _Character)
{

}
