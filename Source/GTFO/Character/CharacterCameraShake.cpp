// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCameraShake.h"
#include "PerlinNoiseCameraShakePattern.h"

UCharacterCameraShake::UCharacterCameraShake(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	UPerlinNoiseCameraShakePattern* Pattern = CreateDefaultSubobject<UPerlinNoiseCameraShakePattern>(TEXT("Pattern"));
	Pattern->Duration = 0.5f;
	Pattern->LocationAmplitudeMultiplier = 7.0f;
	Pattern->LocationFrequencyMultiplier = 7.f;
	Pattern->X.Amplitude = 1.0f;
	Pattern->Y.Amplitude = 1.0f;

	SetRootShakePattern(Pattern);
}