// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/MasterWidget.h"

#include "HPWidget.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UHPWidget : public UMasterWidget
{
	GENERATED_BODY()
	
protected:
	//위젯이 초기화될 때 Hp 프로그래스바의 포인터를 가져오는 기능을 추가해야한다. 
	virtual void NativeConstruct() override;
public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void SetHp(float _HP);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;	

	UPROPERTY()
	float MaxHp;
};
