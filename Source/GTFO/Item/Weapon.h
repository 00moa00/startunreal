// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemData.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class GTFO_API UWeapon : public UItemData
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	//UPROPERTY(EditAnywhere, Category = Weapon)
	//TSoftObjectPtr<USkeletalMesh> ItemMesh;

	//UPROPERTY(EditAnywhere, Category = Stat)
	//FABCharacterStat ModifierStat;
};
