// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BossComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBossComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBossComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;			//몽타주 섹션 이름

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;						//몇개의 콤보 액션이 있는지

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;			//사전에 입력이 되었는지 감지
	
};
