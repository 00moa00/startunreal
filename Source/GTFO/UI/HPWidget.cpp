// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"

void UHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Hp")));

	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
	HpProgressBar->SetPercent(MaxHp / MaxHp);

}

void UHPWidget::SetHp(float _HP)
{
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(_HP / MaxHp);
	}
}
