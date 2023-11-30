// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TerminalItemData.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UTerminalItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	UTerminalItemData();

	UPROPERTY(EditAnywhere, Category = TerminalItem)
	FString ID;

	UPROPERTY(EditAnywhere, Category = TerminalItem)
	FString Name;

	UPROPERTY(EditAnywhere, Category = TerminalItem)
	FString Info;

	UPROPERTY(EditAnywhere, Category = TerminalItem)
	FString Location;
};
