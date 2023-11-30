#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStatData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	FCharacterStatData() : MaxHp(0.0f), JumpZVelocity(0.0f), AirControl(0.0f), MaxWalkSpeed(0.0f), DashPower(0.0f), BrakingDecelerationWalking(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float JumpZVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float DashPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BrakingDecelerationWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Stemina;

	//float가 모여있는 구조체
	// 두개의 구조체를 서로 더했을때 멤버변수가 뭐가 있는지를 검사하고 이것을 더하는 것이 아니고
	// 크기만큼 개수를 확인하고 더해주면 나중에 새로운 연산자를 추가해도 덧셈 연산을 고칠 필요가 없게 된다. 
	FCharacterStatData operator+(const FCharacterStatData& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FCharacterStatData Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FCharacterStatData) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};
