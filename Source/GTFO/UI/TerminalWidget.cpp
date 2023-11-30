// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TerminalWidget.h"
#include "Character/OnTerminal.h"
#include <Components\EditableTextBox.h>

void UTerminalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TarminalText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("TerminalText"))));
	if (TarminalText)
	{
		TarminalText->OnTextCommitted.AddDynamic(this, &UTerminalWidget::OnTextCommitted);
		
		FText MyNewText = FText::FromString("Root>");
		TarminalText->SetText(MyNewText);

		//TarminalText->OnKeyDown.AddDynamic(this, &UTerminalWidget::OnKeyDown);
	}
	
}

void UTerminalWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsFocus)
	{
		if (TarminalText)
		{
			TarminalText->SetKeyboardFocus();

		}

	}
}

void UTerminalWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	//FString InputText = Text.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("Input Text: %s"), *InputText);

	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString CurrentText = *Text.ToString();

		//문자열의 시작 부분에서 Root> 제거

		CurrentText.RemoveFromStart(TEXT("Root>"));

		// '_'를 기준으로 문자열을 분할
		TArray<FString> SplitStrings;
		CurrentText.ParseIntoArray(SplitStrings, TEXT("_"), true);

		
		TerminalData.Broadcast(SplitStrings);
		UE_LOG(LogTemp, Warning, TEXT("Text committed: %s"), *Text.ToString());
	}
}

void UTerminalWidget::SetTerminalDataDelegate(const FOnTerminalData& InDelegate)
{
	TerminalData = InDelegate;
}

void UTerminalWidget::CreateTerminalWidget()
{
	Super::NativeConstruct();

	TarminalText = NewObject<UEditableTextBox>(this, UEditableTextBox::StaticClass());

	if (TarminalText)
	{
		TarminalText->OnTextCommitted.AddDynamic(this, &UTerminalWidget::OnTextCommitted);

		FText MyNewText = FText::FromString("Root>");
		TarminalText->SetText(MyNewText);


		FEditableTextBoxStyle TextBoxStyle = FEditableTextBoxStyle();
		TextBoxStyle.SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
		TextBoxStyle.SetForegroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		TextBoxStyle.SetFont(TEXT("Slate/Fonts/Roboto-Regular.ttf"), 18);
		TarminalText->WidgetStyle = (TextBoxStyle);

	}

}
