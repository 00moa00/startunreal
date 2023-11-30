// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeartWidget.h"
#include "Interface/CharacterWidgetInterface.h"

UHeartWidget::UHeartWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UHeartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

}

void UHeartWidget::SetHp(float _HP)
{
	
}

