// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPSWidgetComponent.h"
#include "UI/MasterWidget.h"

void UFPSWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMasterWidget* MasterWidget = Cast<UMasterWidget>(GetWidget());
	if (MasterWidget)
	{
		if (GetOwner())
		{
			MasterWidget->SetOwningActor(GetOwner());
		}
		
	}
}
