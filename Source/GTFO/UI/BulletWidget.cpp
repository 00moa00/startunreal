// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BulletWidget.h"
#include <Components\EditableTextBox.h>

void UBulletWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BulletCountText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("BullletCount"))));
	FText MyNewText = FText::FromString(TEXT("InputText"));
	BulletCountText->SetText(MyNewText);
}

void UBulletWidget::OnTextChanged(FString& _NewText)
{
	FText NewText = FText::FromString(_NewText);
	BulletCountText->SetText(NewText);

}

void UBulletWidget::OnTextChanged(uint32 _NewText)
{
	FString NewFStringValue = FString::Printf(TEXT("%u"), _NewText);
	OnTextChanged(NewFStringValue);
}
