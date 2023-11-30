// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StaminaWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StaminaProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Stemina")));

	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
	StaminaProgressBar->SetPercent(MaxStamina / MaxStamina);

}

void UStaminaWidget::SetStemina(float _Stemina)
{
	if (StaminaProgressBar)
	{
		CurrentStamina = _Stemina;
		StaminaProgressBar->SetPercent(_Stemina / MaxStamina);
	}
}
