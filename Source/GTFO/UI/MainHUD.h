// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */

UENUM()
enum class EScaleState : uint8
{
	Idle,
	TargetIdle,
	AddScale,
	SubScale
};

UCLASS()
class GTFO_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<class UHPWidget> PlayerHPWidget;

	UPROPERTY()
	TObjectPtr<class UStaminaWidget> PlayerSteminaWidget;

	UPROPERTY()
	TObjectPtr<class USightWidget> SightWidget;

	UPROPERTY()
	TObjectPtr<class UCanvasPanelSlot> CanvasSight;

	UPROPERTY()
	TObjectPtr<class UBulletWidget> BlueBulletWidget;

	UPROPERTY()
	TObjectPtr<class UBulletWidget> GreenBulletWidget;

	UPROPERTY()
	TObjectPtr<class UBulletWidget> RedBulletWidget;

	UPROPERTY()
	TObjectPtr<class UTerminalWidget> TerminalTextWidget;

	UPROPERTY()
	TObjectPtr<class UChipIcon> BlueChip;

	UPROPERTY()
	TObjectPtr<class UChipIcon> GreenChip;

	UPROPERTY()
	TObjectPtr<class UChipIcon> RedChip;

	UPROPERTY()
	TObjectPtr<class UScrollBox> TextScrollBox;

	UPROPERTY()
	TObjectPtr<class UGuideWidget> GuideText;

public:
	FORCEINLINE TObjectPtr<class UHPWidget> GetPlayerHPWidget() { return PlayerHPWidget; }
	FORCEINLINE TObjectPtr<class USightWidget> GetSightWidget() { return SightWidget; }
	FORCEINLINE TObjectPtr<class UStaminaWidget> GetSteminaWidget() { return PlayerSteminaWidget; }

	FORCEINLINE TObjectPtr<class UBulletWidget> GetBlueBulletWidget() { return BlueBulletWidget; }
	FORCEINLINE TObjectPtr<class UBulletWidget> GetGreenBulletWidget() { return GreenBulletWidget; }
	FORCEINLINE TObjectPtr<class UBulletWidget> GetRedBulletWidget() { return RedBulletWidget; }

	FORCEINLINE TObjectPtr<class UCanvasPanelSlot> GetCanvasSight() { return CanvasSight; }

	FORCEINLINE TObjectPtr<class UTerminalWidget> GetTerminalText() { return TerminalTextWidget; }
	FORCEINLINE TObjectPtr<class UScrollBox> GetTextScrollBox() { return TextScrollBox; }

	FORCEINLINE TObjectPtr<class UGuideWidget> GetGuideWidget() { return GuideText; }

	void SetbTargeting(bool _b);
	void AddSightWidgetTransform(float _angle, float _DeltaTime);

	void OnBlueChip();
	void OnGreenChip();
	void OnRedChip();

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;

	void CreateEditableTextBox(FString ID, FString Name, FString Info, FString Location);
	void CreateEditableTextBox(FString OneText);

	void CreateTerminalBox();
	
	float SightScaling;
	bool bTargeting : true;
	EScaleState ScaleState;

	UPROPERTY()
	FVector2D InitScale;

};
