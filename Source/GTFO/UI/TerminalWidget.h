// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MasterWidget.h"
#include "Types/SlateEnums.h"
#include "TerminalWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTerminalData, TArray<FString> /*DataArray*/);

/**
 * 
 */
UCLASS()
class GTFO_API UTerminalWidget : public UMasterWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	
public:
	TObjectPtr <class UEditableTextBox> TarminalText;

	TObjectPtr<class ATerminalItemDataList> TerminalItemDataList;

	FOnTerminalData TerminalData;

	bool IsFocus = true;

	void SetTerminalDataDelegate(const FOnTerminalData& InDelegate);
	void CreateTerminalWidget();

	FORCEINLINE void SetIsFocus(bool _Focus) { IsFocus = _Focus; }
	
};
