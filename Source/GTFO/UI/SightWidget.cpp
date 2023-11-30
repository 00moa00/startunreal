// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

void USightWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Sight = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("sightpanel")));
	//Sight = Cast<UCanvasPanel>(WidgetTree->FindWidget("Image_36"));
	//Sight = Cast<UCanvasPanel>(WidgetTree->FindWidget("sightpanel"));

	//UImage* YourImage = Cast<UImage>(WidgetTree->FindWidget("Image_36"));

	//UCanvasPanelSlot* Canvas = Cast<UCanvasPanelSlot>(YourImage->Slot);
}

void USightWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{

}
