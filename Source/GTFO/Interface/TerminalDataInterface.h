// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/TerminalItemData.h"

#include "TerminalDataInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTerminalDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GTFO_API ITerminalDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual TObjectPtr<class UTerminalItemData> GetTerminalItemData() = 0;
	virtual void OpenDoor() {};
	virtual bool IsDoorOverlap() { return false; }
};
