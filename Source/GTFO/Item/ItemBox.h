// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TerminalDataInterface.h"

#include "ItemBox.generated.h"

UCLASS()
class GTFO_API AItemBox :
	public AActor,
	public ITerminalDataInterface

{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> BoxlidMesh;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> BoxMesh;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UItemData> Item;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

private:
	bool bIsOpen;
	float BoxRatation;
	FRotator BoxWorldRotation;

public:
	UPROPERTY(EditAnywhere, Category = TerminalData, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTerminalItemData> TerminalData;

	virtual TObjectPtr<UTerminalItemData> GetTerminalItemData() { return TerminalData; }

};
