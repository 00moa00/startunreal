// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "CharacterCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UCharacterCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()
	
public:
	UCharacterCameraShake(const FObjectInitializer& ObjectInitializer);

};
