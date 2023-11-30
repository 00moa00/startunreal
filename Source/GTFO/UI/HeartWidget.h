// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MasterWidget.h"
#include "HeartWidget.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UHeartWidget : public UMasterWidget
{
	GENERATED_BODY()
	
public:
	UHeartWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void SetHp(float _HP);

protected:
	UPROPERTY()
	float MaxHp;
};
