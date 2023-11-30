// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MasterWidget.h"

#include "StaminaWidget.generated.h"

/**
 * 
 */



UCLASS()
class GTFO_API UStaminaWidget : public UMasterWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> StaminaProgressBar;

	UPROPERTY()
	float MaxStamina;

	UPROPERTY()
	float CurrentStamina;

public:
	FORCEINLINE void SetMaxStemina(float NewMaxStemina) { MaxStamina = NewMaxStemina; }
	void SetStemina(float _Stemina);
};
