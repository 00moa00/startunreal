// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"
#include "UI/HPWidget.h"
#include "UI/FPSWidgetComponent.h"
#include "UI/SightWidget.h"
#include "UI/ChipIcon.h"
#include "UI/StaminaWidget.h"
#include "UI/BulletWidget.h"
#include "UI/TerminalWidget.h"
#include "UI/GuideWidget.h"

#include <Components\EditableTextBox.h>
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

#include "Blueprint/WidgetTree.h"

void UMainHUD::SetbTargeting(bool _b)
{
	bTargeting = _b;
}

void UMainHUD::AddSightWidgetTransform(float _angle, float _DeltaTime)
{
	float CurrnetAngle = SightWidget->GetRenderTransformAngle();
	SightWidget->SetRenderTransformAngle(CurrnetAngle + (_angle * _DeltaTime));
}

void UMainHUD::OnBlueChip()
{
	BlueChip->SetVisibility(ESlateVisibility::Visible);
	GreenChip->SetVisibility(ESlateVisibility::Hidden);
	RedChip->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUD::OnGreenChip()
{
	BlueChip->SetVisibility(ESlateVisibility::Hidden);
	GreenChip->SetVisibility(ESlateVisibility::Visible);
	RedChip->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUD::OnRedChip()
{
	BlueChip->SetVisibility(ESlateVisibility::Hidden);
	GreenChip->SetVisibility(ESlateVisibility::Hidden);
	RedChip->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerHPWidget = Cast<UHPWidget>(GetWidgetFromName(FName(TEXT("UI_LeftHp"))));
    SightWidget = Cast<USightWidget>(GetWidgetFromName(FName(TEXT("UI_Sight"))));
	PlayerSteminaWidget = Cast<UStaminaWidget>(GetWidgetFromName(FName(TEXT("UI_Stemina"))));

	BlueChip = Cast<UChipIcon>(GetWidgetFromName(FName(TEXT("UI_IconBlue"))));
	GreenChip = Cast<UChipIcon>(GetWidgetFromName(FName(TEXT("UI_IconGreen"))));
	RedChip = Cast<UChipIcon>(GetWidgetFromName(FName(TEXT("UI_IconRed")))); 

	BlueBulletWidget = Cast<UBulletWidget>(GetWidgetFromName(FName(TEXT("UI_BlueBullet"))));
	GreenBulletWidget = Cast<UBulletWidget>(GetWidgetFromName(FName(TEXT("UI_GreenBullet"))));
	RedBulletWidget = Cast<UBulletWidget>(GetWidgetFromName(FName(TEXT("UI_RedBullet"))));

	TerminalTextWidget = Cast<UTerminalWidget>(GetWidgetFromName(FName(TEXT("UI_Terminal"))));

	CanvasSight = Cast<UCanvasPanelSlot>(SightWidget->Slot);

	TextScrollBox = Cast<UScrollBox>(GetWidgetFromName(FName(TEXT("TextScrollBox"))));
	TextScrollBox->SetVisibility(ESlateVisibility::Hidden);
	TextScrollBox->SetScrollBarVisibility(ESlateVisibility::Hidden);

	GuideText = Cast<UGuideWidget>(GetWidgetFromName(FName(TEXT("UI_Guide"))));

	ScaleState = EScaleState::Idle;
	InitScale = SightWidget->GetRenderTransform().Scale;

	SightScaling = 1.0f;

	GreenChip->SetVisibility(ESlateVisibility::Hidden);
	RedChip->SetVisibility(ESlateVisibility::Hidden);

	APawn* Test = GetOwningPlayerPawn();

}

void UMainHUD::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

	switch (ScaleState)
	{
	case EScaleState::Idle:
		if (bTargeting == true)
		{
			ScaleState = EScaleState::SubScale;
		}
		break;

	case EScaleState::TargetIdle:
		break;

	case EScaleState::AddScale:
	{
		SightScaling += 1.0f * _DeltaTime;
		SightWidget->SetRenderScale(InitScale * SightScaling);

		if (SightScaling >= 1.0f)
		{
			SightWidget->SetRenderScale(InitScale);
			SightScaling = 1.0f;
			if (bTargeting == true)
			{
				ScaleState = EScaleState::SubScale;
			}
			else
			{
				ScaleState = EScaleState::Idle;
			}
			break;
		}
		break;  
	}
	case EScaleState::SubScale:
	{

		SightScaling -= 1.0f * _DeltaTime;
		SightWidget->SetRenderScale(InitScale * SightScaling);

		if (SightScaling <= 0.7f)
		{
			SightWidget->SetRenderScale(InitScale * 0.7f);
			SightScaling = 0.7f;
			if (bTargeting == true)
			{
				ScaleState = EScaleState::AddScale;
			}
			else
			{
				ScaleState = EScaleState::Idle;
			}
			break;
		}

		if (bTargeting == false)
		{
			ScaleState = EScaleState::AddScale;
		}
		break;
	}
	default:
		break;
	}
}

void UMainHUD::CreateEditableTextBox(FString ID, FString Name, FString Info, FString Location)
{
	FString NewText = ID;
	for (int a = 0; a < 25 - ID.Len(); a++)
	{
		NewText += "  ";
	}

	NewText += Name;

	for (int a = 0; a < 25 - Name.Len(); a++)
	{
		NewText += "  ";
	}

	NewText += Info;


	for (int a = 0; a < 25 - Info.Len(); a++)
	{
		NewText += "  ";
	}

	NewText += Location;

	for (int a = 0; a < 25 - Location.Len(); a++)
	{
		NewText += "  ";
	}

	FText NewFText = FText::FromString(NewText);


	UEditableTextBox* DynamicEditableTextBox = NewObject<UEditableTextBox>(this, UEditableTextBox::StaticClass());

	if (DynamicEditableTextBox)
	{
		// UEditableTextBox 설정
		DynamicEditableTextBox->SetText(NewFText);
		DynamicEditableTextBox->SetIsReadOnly(true);
		FEditableTextBoxStyle TextBoxStyle = FEditableTextBoxStyle();
		TextBoxStyle.SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		TextBoxStyle.SetFocusedForegroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

		TextBoxStyle.SetFont(TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18);

		DynamicEditableTextBox->WidgetStyle = TextBoxStyle;

		// Widget에 추가
		TextScrollBox->AddChild(DynamicEditableTextBox);
	}
}

void UMainHUD::CreateEditableTextBox(FString OneText)
{
	FText NewFText = FText::FromString(OneText);

	UEditableTextBox* DynamicEditableTextBox = NewObject<UEditableTextBox>(this, UEditableTextBox::StaticClass());

	if (DynamicEditableTextBox)
	{
		// UEditableTextBox 설정
		DynamicEditableTextBox->SetText(NewFText);
		DynamicEditableTextBox->SetIsReadOnly(true);
		FEditableTextBoxStyle TextBoxStyle = FEditableTextBoxStyle();
		TextBoxStyle.SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		TextBoxStyle.SetForegroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		TextBoxStyle.SetFocusedForegroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		TextBoxStyle.SetFont(TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18);


		DynamicEditableTextBox->WidgetStyle = TextBoxStyle;

		// Widget에 추가
		TextScrollBox->AddChild(DynamicEditableTextBox);
	}
}

void UMainHUD::CreateTerminalBox()
{
	TerminalTextWidget->TarminalText->SetIsReadOnly(true);
	TerminalTextWidget->SetIsFocus(false);
	TerminalTextWidget->TerminalData.Clear();

	UTerminalWidget* NewTerminalWidget = NewObject<UTerminalWidget>(this, UTerminalWidget::StaticClass());

	if (NewTerminalWidget)
	{
		// Widget에 추가
		NewTerminalWidget->CreateTerminalWidget();
		TerminalTextWidget = NewTerminalWidget;
		TextScrollBox->AddChild(NewTerminalWidget->TarminalText);
		TerminalTextWidget->TarminalText->SetIsReadOnly(false);

		TerminalTextWidget->SetIsFocus(true);
	}
}



