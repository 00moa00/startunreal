// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TerminalItemDataList.generated.h"

//USTRUCT(BlueprintType)
//struct FDataInfo
//{
//	GENERATED_BODY()
//
//	
//	FString Name;
//	FString Info;
//	FString Location;
//};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GTFO_API UTerminalItemDataList : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTerminalItemDataList();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	TArray<TObjectPtr<class UTerminalItemData>> DataList;

};
