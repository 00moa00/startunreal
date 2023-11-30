// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GuideWidget.h"
#include <Components\EditableTextBox.h>

void UGuideWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MainText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("MainText"))));
	SubAText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("SubAText"))));
	SubBText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("SubBText"))));

	MainText->SetVisibility(ESlateVisibility::Hidden);
	SubAText->SetVisibility(ESlateVisibility::Hidden);
	SubBText->SetVisibility(ESlateVisibility::Hidden);

}

void UGuideWidget::OnGuideTextChange(FString _MainText, FString _SubTextA, FString _SubTextB)
{
	MainText->SetVisibility(ESlateVisibility::Visible);
	SubAText->SetVisibility(ESlateVisibility::Visible);
	SubBText->SetVisibility(ESlateVisibility::Visible);

	FText NewMainText = FText::FromString(_MainText);
	MainText->SetText(NewMainText);

	FText NewSubAText = FText::FromString(_SubTextA);
	SubAText->SetText(NewSubAText);

	FText NewSubBText = FText::FromString(_SubTextB);
	SubBText->SetText(NewSubBText);

	GetWorld()->GetTimerManager().SetTimer(TextTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			MainText->SetVisibility(ESlateVisibility::Hidden);
			SubAText->SetVisibility(ESlateVisibility::Hidden);
			SubBText->SetVisibility(ESlateVisibility::Hidden);
		}
	), 3.0, false);
}