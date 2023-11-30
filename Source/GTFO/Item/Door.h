// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TerminalDataInterface.h"

#include "Door.generated.h"

UCLASS()
class GTFO_API ADoor :
	public AActor,
	public ITerminalDataInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<class UStaticMeshComponent> DoorMesh;

	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr <class ICharacterItemInterface> OverlappingPawn;

	UPROPERTY(EditAnywhere, Category = Door)
	FVector OpenDirection;

	bool bIsOpen = false;
	bool bIsOverlap = false;
	float OpenTimer = 0.f;
	FVector MoveLocation;

public:
	UPROPERTY(EditAnywhere, Category = TerminalData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTerminalItemData> TerminalData;

	virtual TObjectPtr<UTerminalItemData> GetTerminalItemData() { return TerminalData; }
	virtual void OpenDoor() override;
	virtual bool IsDoorOverlap() override;

};
