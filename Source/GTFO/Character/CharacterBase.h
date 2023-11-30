// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"

#include "Interface/CharacterWidgetInterface.h"
#include "Interface/CharacterItemInterface.h"
#include "GameFramework/Character.h"
//#include "AI/GTFOAI.h"

#include "CharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCharacter, Log, All);

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UItemData* /*InItemData*/);

DECLARE_DELEGATE(FOnChangeCharacterDelegate);
DECLARE_DELEGATE(FShootingDelegate);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater,
	FPS
};

UENUM()
enum class ECharacterType : uint8
{
	MIN ,
	Blue,
	Green,
	Red,
	MAX
};


USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

USTRUCT(BlueprintType)
struct FChangeCharacterWrapper
{
	GENERATED_BODY()
	FChangeCharacterWrapper() {}
	FChangeCharacterWrapper(const FOnChangeCharacterDelegate& InChangeDelegate) : CharacterChangeDelegate(InChangeDelegate) {}
	FOnChangeCharacterDelegate CharacterChangeDelegate;
};

USTRUCT(BlueprintType)
struct FShootingWrapper
{
	GENERATED_BODY()
	FShootingWrapper() {}
	FShootingWrapper(const FShootingDelegate& InShootingDelegate) : ShootingDelegate(InShootingDelegate) {}
	FShootingDelegate ShootingDelegate;
};

UCLASS()
class GTFO_API ACharacterBase :
	public ACharacter, 
	public ICharacterWidgetInterface, 
	public ICharacterItemInterface

{
	GENERATED_BODY()
	
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//USkeletalMeshComponent* PlayerMesh;

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
public:	
	virtual void Tick(float DeltaTime) override;

	//Movement
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float DashSpeed;

	// Character, Animation Section
protected:
	TObjectPtr<class AFPSPlayerController> FPSPlayerController;


	UPROPERTY(EditAnywhere, Category = Character)
	TObjectPtr<class USpotLightComponent> FlashLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitMontage;

	TObjectPtr<class UFPSAnimInstance> AnimInst;

	ECharacterType CurrentCharacterType;
	//ECharacterBehaviorType BehaviorType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class USkeletalMesh> BlueMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class USkeletalMesh> RedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class USkeletalMesh> GreenMesh;


	UPROPERTY()
	TArray<FShootingWrapper> CharacterShootings;

	UPROPERTY()
	TArray<FChangeCharacterWrapper> ChangeCharacterActions;

	bool isShootLimitTime = false;
	bool isGreenShootingHold = false;
	bool isGreenShootingOne = true;
	bool isDashSoundCheck = false;
	bool isWalkSoundCheck = false;
	bool isSpectrumTime = false;
	bool isSpectrumCreateTime = false;
	bool isLight = true;
	bool LightTimer = false;
	bool isInteraction = false;
	bool IsHasHealKit = false;

	FTimerHandle DeshTimerHandle;
	FTimerHandle WalkTimerHandle;
	FTimerHandle SpectrumHandle;
	FTimerHandle SpectrumCreateHandle;
	FTimerHandle LightTimerHandle;

	FVector2D InputMovementVector;
	FVector SaveMuzzleLocation;
	FRotator SaveSpawnRotation;

	FTimerHandle HoldTimerHandle;

	UPROPERTY()
	TObjectPtr <class AParticleNiagara> GreenBulletHoldParticle;

	//virtual void SetBehaviorDelegate(const FOnCharacterBehavior& BehaviorDelegate) override;

	void ChangeBlueCharacter();
	void ChangeGreenCharacter();
	void ChangeRedCharacter();

	void BlueShoot();
	void BlueShootCombo(FVector _Location, bool _isHit);
	void BlueShootEnd(FVector _Location, bool _isHit);
	
	void GreenShoot();
	void GreenReleaseShoot();
	void GreenShooting();
	void GreenExplosion(FVector _Location, bool _isHit);

	void RedShoot();

	void SpectrumEffect();

public:
	//FORCEINLINE ECharacterBehaviorType GetBehaviorType() const { return BehaviorType; }
	TObjectPtr<class UFPSAnimInstance> GetCharacterAnimInstance() {return AnimInst;}


	//Sound

public:
	TObjectPtr <class USoundBase> ShootBgm;


	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCameraComponent;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UCharacterControlData*> CharacterControlManager;


	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	ECharacterControlType CurrentCharacterControlType;

	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	void SetCharacterControlData(const class UCharacterControlData* CharacterControlData) ;

public:
	void ShakeCamera();

	// Input Section
protected:
	/** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShootingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReleaseShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReleaseDashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReleaseShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FPSMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FPSLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ViewChangeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WheelUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WheelDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SpectrumAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RaleaseSpectrumAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReleaseInteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> HealAction;

	void QuaterMove(const FInputActionValue& Value);

	void ShoulderMove(const FInputActionValue& Value);
	void ReleaseShoulderMove(const FInputActionValue& Value);

	void ShoulderLook(const FInputActionValue& Value);

	void FPSMove(const FInputActionValue& Value);
	void FPSLook(const FInputActionValue& Value);

	void Shoot();
	void StopShoot();
	void Shooting();

	void Dash(const FInputActionValue& Value);
	void ReleaseDash(const FInputActionValue& Value);

	void ItemChangeUp();
	void ItemChangeDown();

	void SpectrumTime();
	void ReleaseSpectrumTime();

	void Interaction();
	void ReleaseInteraction();

	void Light();

	void Heal();

	//bool InPressedSpectrum = false;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetCharacterInput();

	//Collision
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void isPossibelAttack(float _DeltaTime);

	UFUNCTION()
	void LightVsMonster(float _DeltaTime);

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharacterStatComponent> Stat;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// UI Widget Section
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> UI_Heart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMainHUD> UI_MainHUD;

	virtual void SetupCharacterWidget(class UMasterWidget* InUserWidget) override;

	// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBulletDataComponent> BulletData;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual bool GetIsInteractionAction() override;
	virtual void TakeItem(class UItemData* InItemData) override;

	void TakeHealKit(class UItemData* InItemData);
	void TakeBlueBullet(class UItemData* InItemData);
	void TakeGreenBullet(class UItemData* InItemData);
	void TakeRedBullet(class UItemData* InItemData);
	void TakeKey(class UItemData* InItemData);
};