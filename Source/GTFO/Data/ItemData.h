// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	BlueBullet,
	GreenBullet,
	RedBullet,
	HealKit,
	Key
};


UCLASS()
class GTFO_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

	//UFUNCTION()
	//TSoftObjectPtr<UStaticMeshComponent> GetItemMesh() { return ItemMeshComponent; }

public:
	//UPROPERTY(EditAnywhere, Category = Weapon)
	//TSoftObjectPtr<UStaticMeshComponent> ItemMeshComponent;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;

	UPROPERTY(EditAnywhere, Category = Count)
	uint32 Count;
};
