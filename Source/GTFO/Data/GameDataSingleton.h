// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/CharacterStatData.h"
#include "Data/MonsterData.h"
#include "Data/BulletData.h"
#include "GameDataSingleton.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

UCLASS()
class GTFO_API UGameDataSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UGameDataSingleton();
	static UGameDataSingleton& GetInst();

	//Character Stat Data Section
public:

	//테이블에서 레벨에 해당하는 하나의 구조체 정보를 리턴하도록
public:
	FORCEINLINE FCharacterStatData GetCharacterStatData(int32 _InLevel) const { return CharacterStatTableValue.IsValidIndex(_InLevel - 1) ? CharacterStatTableValue[_InLevel - 1] : FCharacterStatData(); }
	FORCEINLINE FCharacterStatData GetCharacterStatData(FName _Name) const {return *CharacterStatTableKeyAndValue.Find(_Name);}

	FORCEINLINE FMonsterData GetMonsterData(FName _Name) const { return *MonsterDataTableKeyAndValue.Find(_Name); }
	
	FORCEINLINE FBulletData GetBulletData(FName _Name) const { return *BulletDataTableKeyAndValue.Find(_Name); }


	UPROPERTY()
	int32 CharacterMaxLevel;	//몇 개의 레벨을 가지고있는지


private:
	TArray<FCharacterStatData> CharacterStatTableValue;
	TMap<FName,FCharacterStatData> CharacterStatTableKeyAndValue;

	TMap<FName, FMonsterData> MonsterDataTableKeyAndValue;

	TMap<FName, FBulletData> BulletDataTableKeyAndValue;

};
