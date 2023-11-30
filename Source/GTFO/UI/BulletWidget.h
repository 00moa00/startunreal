// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MasterWidget.h"
#include "BulletWidget.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UBulletWidget : public UMasterWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	TObjectPtr <class UEditableTextBox> BulletCountText;

	void OnTextChanged( FString& NewText);
	void OnTextChanged(uint32 NewText);
};
