// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MasterWidget.h"
#include "Blueprint/UserWidget.h"
#include "SightWidget.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API USightWidget : public UMasterWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;


public:
	class UCanvasPanel* GetSight(){return Sight; }

protected:
	UPROPERTY()
	TObjectPtr<class UCanvasPanel> Sight;
};
