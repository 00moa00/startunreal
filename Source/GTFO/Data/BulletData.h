#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BulletData.generated.h"

USTRUCT(BlueprintType)
struct FBulletData: public FTableRowBase
{
	GENERATED_BODY()

	public:
	FBulletData() : Attack(0.0f), Speed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float LifeSpan;

};
