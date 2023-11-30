// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MasterWidget.h"
#include "GuideWidget.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UGuideWidget : public UMasterWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	TObjectPtr <class UEditableTextBox> MainText;
	TObjectPtr <class UEditableTextBox> SubAText;
	TObjectPtr <class UEditableTextBox> SubBText;

	FTimerHandle TextTimerHandle;


	void OnGuideTextChange(FString _MainText, FString _SubTextA = " ", FString _SubTextB = " ");
};
