// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "FPSPlayerController.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CanvasPanelSlot.h"

#include "Camera/CameraShake.h"
#include "Camera/CameraComponent.h"
#include "MatineeCameraShake.h"
#include "Character/CharacterCameraShake.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Character/CharacterControlData.h"
#include "Animation/FPSAnimInstance.h"
#include "Character/CharacterStatComponent.h"
#include "Item/BulletDataComponent.h"
#include "Components/SpotLightComponent.h"

#include "UI/FPSWidgetComponent.h"
#include "UI/HeartWidget.h"
#include "UI/HPWidget.h"
#include "UI/MainHUD.h"
#include "UI/StaminaWidget.h"
#include "UI/BulletWidget.h"
#include "UI/GuideWidget.h"
#include "Components/ScrollBox.h"
#include "UI/SightWidget.h"

#include "Particle/ParticleNiagara.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Data/ItemData.h"

#include "Item/WeaponProjectile.h"
#include "Physics/FPSCollision.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Math/UnrealMathUtility.h" 

#include "Particle/SpectrumEffect.h"

#include "AI/BTService_LevelCheck.h"
#include "Monster/FloatRobot.h"




DEFINE_LOG_CATEGORY(LogCharacter);

// Sets default values
ACharacterBase::ACharacterBase()
{
	//Initialize
	DashSpeed = 700.0f;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Stat Component 
	Stat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));
	BulletData = CreateDefaultSubobject<UBulletDataComponent>(TEXT("BulletData"));
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("PointLight"));
	FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("Muzzle_03"), ERelativeTransformSpace::RTS_Component);
	FlashLight->SetupAttachment(GetMesh(), FName("Muzzle_03"));
	FlashLight->SetWorldTransform(SocketTransform);

	FlashLight->Intensity = 5000.0f;
	FlashLight->InnerConeAngle = 20.0f;
	FlashLight->OuterConeAngle = 40.0f;
	FlashLight->LightColor = FColor::White;
	FlashLight->SetMobility(EComponentMobility::Movable);

	//Stat->SetLevelStat(1);

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterBase::OnOverlapBegin);

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.04f;						//캐릭터가 공중에 있는 경우 점프나 낙하 중에 어떻게 조종되는지 제어하는 속성. 높을수록 공중에서 더 쉽게 움직일 수 있다.
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.f;	//반대 방향으로 이동하려 했을 때 감속


	//// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	// Create a follow camera
	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCameraComponent->SetupAttachment(GetCapsuleComponent()); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCameraComponent->bUsePawnControlRotation = true; // Camera does not rotate relative to arm
	//FollowCameraComponent->SetCollisionResponseToAllChannels(ECollisionEnabled::QueryAndPhysics);
	//control
	static ConstructorHelpers::FObjectFinder<UCharacterControlData> ShoulderDataRef(TEXT("/Script/GTFO.CharacterControlData'/Game/GTFO/Input/CD_Shoulder.CD_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCharacterControlData> QuaterDataRef(TEXT("/Script/GTFO.CharacterControlData'/Game/GTFO/Input/CD_Quater.CD_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCharacterControlData> FPSDataRef(TEXT("/Script/GTFO.CharacterControlData'/Game/GTFO/Input/CD_FPS.CD_FPS'"));
	if (FPSDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::FPS, FPSDataRef.Object);
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GreenMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Skins/ODGreen/Meshes/Wraith_ODGreen.Wraith_ODGreen'"));
	GreenMesh = GreenMeshRef.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RedMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Skins/LunarOps/Meshes/Wraith_LunarOps.Wraith_LunarOps'"));
	RedMesh = RedMeshRef.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BlueMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'"));
	BlueMesh = BlueMeshRef.Object;

	//animaion
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/GTFO/Animation/ABP_PlayerAnimaion.ABP_PlayerAnimaion_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/GTFO/Animation/AM_Hit.AM_Hit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}


	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	CurrentCharacterType = ECharacterType::Blue;

	// Widget Component 
	//Main Hud


	//HPWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);

	//UI_Heart = CreateDefaultSubobject<UFPSWidgetComponent>(TEXT("Widget"));
	
	
	
	//UI_Heart->SetRoca
	//UI_Heart->SetupAttachment(GetMesh());
	//UI_Heart->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/GTFO/UI/UI_PlayerHpHeart.UI_PlayerHpHeart_C"));
	//if (HpBarWidgetRef.Class)
	//{
	//	UI_Heart->SetWidgetClass(HpBarWidgetRef.Class);
	//	UI_Heart->SetWidgetSpace(EWidgetSpace::Screen);
	//	//UI_Heart->SetDrawSize(FVector2D(150.0f, 15.0f));
	//	UI_Heart->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

	CameraShakeClass = UCameraShakeBase::StaticClass();

	// Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACharacterBase::TakeBlueBullet)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACharacterBase::TakeGreenBullet)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACharacterBase::TakeRedBullet)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACharacterBase::TakeHealKit)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ACharacterBase::TakeKey)));

	ChangeCharacterActions.Add(FChangeCharacterWrapper(FOnChangeCharacterDelegate::CreateUObject(this, &ACharacterBase::ChangeBlueCharacter)));
	ChangeCharacterActions.Add(FChangeCharacterWrapper(FOnChangeCharacterDelegate::CreateUObject(this, &ACharacterBase::ChangeGreenCharacter)));
	ChangeCharacterActions.Add(FChangeCharacterWrapper(FOnChangeCharacterDelegate::CreateUObject(this, &ACharacterBase::ChangeRedCharacter)));

	CharacterShootings.Add(FShootingWrapper(FShootingDelegate::CreateUObject(this, &ACharacterBase::BlueShoot)));
	CharacterShootings.Add(FShootingWrapper(FShootingDelegate::CreateUObject(this, &ACharacterBase::GreenShoot)));
	CharacterShootings.Add(FShootingWrapper(FShootingDelegate::CreateUObject(this, &ACharacterBase::RedShoot)));


	//TObjectPtr<class UWidgetComponent> Test = CreateDefaultSubobject<UFPSWidgetComponent>(TEXT("WidgetTest"));
	////Test->SetWorldLocation(FVector(0.0f, 50.f, 0.0f));
	//static ConstructorHelpers::FClassFinder<UUserWidget> TestRef(TEXT("/Game/GTFO/UI/UI_Sight.UI_Sight_C"));
	//if (TestRef.Class)
	//{
	//	
	//	//Test->SetRoot
	//	Test->SetWidgetClass(TestRef.Class);
	//	Test->SetWidgetSpace(EWidgetSpace::Screen);
	//	//UI_Heart->SetDrawSize(FVector2D(150.0f, 15.0f));
	//	//Test->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
	AnimInst = Cast<UFPSAnimInstance>(GetMesh()->GetAnimInstance());

	FPSPlayerController = CastChecked<AFPSPlayerController>(GetController());
	FPSPlayerController->SetCharacterPawn(this);

	//Main Hud
	//AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	UI_MainHUD = FPSPlayerController->GetMainHUD();
	if (UI_MainHUD)
	{
		UI_MainHUD->GetPlayerHPWidget()->SetMaxHp(Stat->GetTotalStat().MaxHp);
		UI_MainHUD->GetPlayerHPWidget()->SetHp(Stat->GetTotalStat().MaxHp);
		Stat->OnHpChanged.AddUObject(UI_MainHUD->GetPlayerHPWidget(), &UHPWidget::SetHp); //Broadcast 가 될떄 불러와진다.
		//Stat->ApplyDamage(50);

		UI_MainHUD->GetSteminaWidget()->SetMaxStemina(Stat->GetTotalStat().Stemina);
		UI_MainHUD->GetSteminaWidget()->SetStemina(Stat->GetTotalStat().Stemina);
		Stat->OnSteminaChanged.AddUObject(UI_MainHUD->GetSteminaWidget(), &UStaminaWidget::SetStemina); 

		Stat->OnRedBulletChanged.AddUObject(UI_MainHUD->GetRedBulletWidget(), &UBulletWidget::OnTextChanged);
		Stat->OnBlueBulletChanged.AddUObject(UI_MainHUD->GetBlueBulletWidget(), &UBulletWidget::OnTextChanged);
		Stat->OnGreenBulletChanged.AddUObject(UI_MainHUD->GetGreenBulletWidget(), &UBulletWidget::OnTextChanged);

		Stat->AddBlueBullet(20);
		Stat->AddGreenBullet(20);
		Stat->AddRedBullet(20);
	}

	//FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle_03"));
	//FlashLight->SetRelativeLocation(MuzzleLocation);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UI_MainHUD)
	{
		isPossibelAttack(DeltaTime);

	}
	LightVsMonster(DeltaTime);
	//ShakeCamera();
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	if (UI_MainHUD)
	{
		UI_MainHUD->GetTextScrollBox()->SetVisibility(ESlateVisibility::Hidden);
		UI_MainHUD->GetSightWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACharacterBase::ChangeBlueCharacter()
{
	if (BlueMesh)
	{
		GetMesh()->SetSkeletalMesh(BlueMesh);
		UI_MainHUD->OnBlueChip();
	}
}

void ACharacterBase::ChangeRedCharacter()
{
	if (RedMesh)
	{
		GetMesh()->SetSkeletalMesh(RedMesh);
		UI_MainHUD->OnRedChip();
	}
}

void ACharacterBase::ChangeGreenCharacter()
{
	if (GreenMesh)
	{
		GetMesh()->SetSkeletalMesh(GreenMesh);
		UI_MainHUD->OnGreenChip();
	}
}

void ACharacterBase::ChangeCharacterControl()
{
	switch (CurrentCharacterControlType)
	{
	case ECharacterControlType::Shoulder:
		SetCharacterControl(ECharacterControlType::FPS);

		break;
	case ECharacterControlType::FPS:
		SetCharacterControl(ECharacterControlType::Shoulder);
		break;
	default:
		break;
	}
}

void ACharacterBase::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	CurrentCharacterControlType = NewCharacterControlType;
	SetCharacterInput();
}

void ACharacterBase::SetCharacterInput()
{
	UCharacterControlData* NewCharacterControl = CharacterControlManager[CurrentCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//기존 매핑 클리어
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

void ACharacterBase::SetCharacterControlData(const UCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	/// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ACharacterBase::ShakeCamera()
{
	FPSPlayerController->PlayerCameraManager->StartCameraShake(UCharacterCameraShake::StaticClass(), 5.0f);
}

void ACharacterBase::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AnimInst->SetMoveDir(MovementVector);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);

		//if (isSpectrumTime == true)
		//{
		//	SpectrumEffect(MovementVector);
		//}

		if (isWalkSoundCheck == false)
		{
			isWalkSoundCheck = true;

			//사운드
			ASound* AShootSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
			AShootSound->SetSoundType(ESoundScale::Scale1, ESoundLevel::Level1);

			GetWorld()->GetTimerManager().SetTimer(WalkTimerHandle, FTimerDelegate::CreateLambda(
				[&]()
				{
					isWalkSoundCheck = false;
				}
			), 1.0f, false);
		}
	}
}

void ACharacterBase::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		AnimInst->SetControllerRotation(GetControlRotation());
	}
}

void ACharacterBase::FPSMove(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	InputMovementVector = MovementVector;
	if (Controller != nullptr)
	{
		// add movement 
		AnimInst->SetMoveDir(MovementVector);

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACharacterBase::FPSLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

		AddControllerYawInput(-LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		//FMath::Clamp(-180, 180, GetControlRotation().Pitch) ; 
		//UE_LOG(LogTemp, Log, TEXT("Character Stamina :: %f"), GetControlRotation().Pitch);

		AnimInst->SetControllerRotation(GetControlRotation());

		//UE_LOG(LogTemp, Log, TEXT("Character Stamina :: %f"), FMath::Clamp(GetControlRotation().Pitch, -180, 180));
	}
}

void ACharacterBase::GreenShoot()
{
	UE_LOG(LogCharacter, Log, TEXT("GreenShoot"));

	if (isGreenShootingHold == false)
	{
		if (Stat->SubGreenBullet(1) == false)
		{
			return;
		}

		GreenBulletHoldParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), SaveMuzzleLocation, SaveSpawnRotation);
		GreenBulletHoldParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_sparking.NS_ky_sparking'"));
		GreenBulletHoldParticle->GetParticleComponent()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);


		GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{
				if(HoldTimerHandle.IsValid())
				{
					isGreenShootingHold = true;
					GreenShooting();
				}			
			}
		), 1.0f, false);
	}
}

void  ACharacterBase::GreenShooting()
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	FRotator SpawnRotation = GetActorRotation();

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const FVector CameraStart = FollowCameraComponent->GetComponentLocation();
	const FVector CameraEnd = CameraStart + FollowCameraComponent->GetForwardVector() * 7000.f;

	FVector CollisionLocation = OutHitResult.Location;

	bool HitDetected = GetWorld()->LineTraceSingleByChannel(OutHitResult, CameraStart, CameraEnd, ECC_WorldStatic, Params);
	if (HitDetected)
	{
		CollisionLocation = OutHitResult.Location;
		//DrawDebugCapsule(GetWorld(), CollisionLocation, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);
	}

	else
	{
		CollisionLocation = FollowCameraComponent->GetForwardVector();
	}

	FVector TargetDirection = (CollisionLocation - MuzzleLocation).GetSafeNormal();
	SpawnRotation = TargetDirection.Rotation();

	SaveMuzzleLocation = MuzzleLocation;
	SaveSpawnRotation = SpawnRotation;

	AWeaponProjectile* NewBullet = GetWorld()->SpawnActor<AWeaponProjectile>(AWeaponProjectile::StaticClass(), SaveMuzzleLocation, SaveSpawnRotation);
	NewBullet->SetBulletInfo(BulletData->GetGreenBulletData().Speed, BulletData->GetGreenBulletData().Attack, BulletData->GetGreenBulletData().LifeSpan);
	NewBullet->SetShouldBounce(false);
	NewBullet->OnDestoryLocation.AddUObject(this, &ACharacterBase::GreenExplosion);
	NewBullet->SetProjectileType(EProjectileType::Green);	//green 등록

	//파티클 attach
	GreenBulletHoldParticle->GetParticleComponent()->AttachToComponent(NewBullet->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
;
}

void ACharacterBase::GreenExplosion(FVector _Location, bool _isHit)
{
	const FRotator SpawnRotation = GetActorRotation();

	//총알 폭발 파티클
	AParticleNiagara* BulletexplosionParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), _Location, SpawnRotation);
	if (_isHit == false)
	{
		ASound* AShootSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
		AShootSound->SetSoundType(ESoundScale::Scale3, ESoundLevel::Level3);

		BulletexplosionParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_sparking2.NS_ky_sparking2'"));
	}
	else
	{
		BulletexplosionParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_sparkShock.NS_ky_sparkShock'"));
	}

	BulletexplosionParticle->SetParticleFinishedDestory();

	//FVector NewScale = FVector(3.0f, 3.0f, 3.0f);
	//BulletexplosionParticle->GetParticleComponent()->SetRelativeScale3D(NewScale);
}

void ACharacterBase::GreenReleaseShoot()
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	const FRotator SpawnRotation = GetActorRotation();

	AWeaponProjectile* NewBullet = GetWorld()->SpawnActor<AWeaponProjectile>(AWeaponProjectile::StaticClass(), MuzzleLocation, SpawnRotation);
	NewBullet->SetBulletInfo(BulletData->GetRedBulletData().Speed, BulletData->GetRedBulletData().Attack, BulletData->GetRedBulletData().LifeSpan);
	NewBullet->SetShouldBounce(false);

	AParticleNiagara* BulletParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), MuzzleLocation, SpawnRotation);
	BulletParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_sparking.NS_ky_sparking'"));
	BulletParticle->GetParticleComponent()->AttachToComponent(NewBullet->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	NewBullet->OnDestory.AddUObject(BulletParticle, &AParticleNiagara::ParticleDestory);
	NewBullet->OnDestoryLocation.AddUObject(this, &ACharacterBase::GreenExplosion);
}

void ACharacterBase::BlueShoot()
{
	//사운드
	ASound* AShootSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
	AShootSound->SetSoundType(ESoundScale::Scale1, ESoundLevel::Level3);

	AWeaponProjectile* NewBullet = GetWorld()->SpawnActor<AWeaponProjectile>(AWeaponProjectile::StaticClass(), SaveMuzzleLocation, SaveSpawnRotation);
	NewBullet->GetCollisionComp()->InitSphereRadius(30.f);
	NewBullet->SetBulletInfo(BulletData->GetBlueBulletData().Speed, BulletData->GetBlueBulletData().Attack, BulletData->GetBlueBulletData().LifeSpan);

	//총알 파티클
	AParticleNiagara* BulletParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), SaveMuzzleLocation, SaveSpawnRotation);
	BulletParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_prazmaBall.NS_ky_prazmaBall'"));
	BulletParticle->GetParticleComponent()->AttachToComponent(NewBullet->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector NewScale = FVector(1.0f, 1.0f, 1.0f);
	BulletParticle->GetParticleComponent()->SetRelativeScale3D(NewScale);

	//총알이 폭발하면 알림. 위치 알림.
	NewBullet->OnDestory.AddUObject(BulletParticle, &AParticleNiagara::ParticleDestory);
	NewBullet->OnDestoryLocation.AddUObject(this, &ACharacterBase::BlueShootCombo);

}

void ACharacterBase::BlueShootCombo(FVector _Location, bool _isHit)
{
	const FRotator SpawnRotation = GetActorRotation();

	//총알 폭발 파티클
	AParticleNiagara* BulletexplosionParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), _Location, SpawnRotation);
	BulletexplosionParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_blackFlare2.NS_ky_blackFlare2'"));
	BulletexplosionParticle->SetParticleFinishedDestory();

	FVector NewScale = FVector(3.0f, 3.0f, 3.0f);
	BulletexplosionParticle->GetParticleComponent()->SetRelativeScale3D(NewScale);

	FVector ProjectileDirection = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));

	for (int a = 0; a < 5; ++a)
	{
		//새 총알

		AWeaponProjectile* NewBullet = GetWorld()->SpawnActor<AWeaponProjectile>(AWeaponProjectile::StaticClass(), _Location, SpawnRotation);
		NewBullet->GetCollisionComp()->InitSphereRadius(40.f);
		NewBullet->SetBulletInfo(BulletData->GetBlueBulletData().Speed, BulletData->GetBlueBulletData().Attack, BulletData->GetBlueBulletData().LifeSpan);


		//총알 파티클
		AParticleNiagara* BulletParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), _Location, SpawnRotation);
		BulletParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_prazmaBall.NS_ky_prazmaBall'"));
		BulletParticle->GetParticleComponent()->AttachToComponent(NewBullet->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
		FVector BulletParticleScale = FVector(0.5f, 0.5f, 0.5f);
		BulletParticle->GetParticleComponent()->SetRelativeScale3D(BulletParticleScale);

		NewBullet->OnDestory.AddUObject(BulletParticle, &AParticleNiagara::ParticleDestory);
		NewBullet->OnDestoryLocation.AddUObject(this, &ACharacterBase::BlueShootEnd);

		NewBullet->SetMovementDirection(ProjectileDirection);
	}

	// UE_LOG(LogCharacter, Log, TEXT("BlueShootCombo"));
}

void ACharacterBase::BlueShootEnd(FVector _Location, bool _isHit)
{
	//총알 폭발 파티클
	AParticleNiagara* BulletexplosionParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), _Location, GetActorRotation());
	BulletexplosionParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/KY_MagicEffects02/Particles/Niagara/NS_ky_blackFlare2.NS_ky_blackFlare2'"));
	BulletexplosionParticle->SetParticleFinishedDestory();

	FVector NewScale = FVector(2.0f, 2.0f, 2.0f);
	BulletexplosionParticle->GetParticleComponent()->SetVectorParameter("Scale", NewScale);

	//사운드
	ASound* AShootSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), _Location, GetActorRotation());
	AShootSound->SetSoundType(ESoundScale::Scale1, ESoundLevel::Level3);
}

void ACharacterBase::RedShoot()
{
	UE_LOG(LogCharacter, Log, TEXT("RedShoot"));
	//사운드
	ASound* AShootSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
	AShootSound->SetSoundType(ESoundScale::Scale1, ESoundLevel::Level3);

	AWeaponProjectile* NewBullet = GetWorld()->SpawnActor<AWeaponProjectile>(AWeaponProjectile::StaticClass(), SaveMuzzleLocation, SaveSpawnRotation);
	NewBullet->SetBulletInfo(BulletData->GetRedBulletData().Speed, BulletData->GetRedBulletData().Attack, BulletData->GetRedBulletData().LifeSpan);
	NewBullet->SetShouldBounce(false);
	//NewBullet->SetMovementDirection(CollisionLocation);

	AParticleNiagara* BulletParticle = GetWorld()->SpawnActor<AParticleNiagara>(AParticleNiagara::StaticClass(), SaveMuzzleLocation, SaveSpawnRotation);
	BulletParticle->SetParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/sA_ShootingVfxPack/FX/NiagaraSystems/NS_AR_Muzzleflash_2_INFINITE.NS_AR_Muzzleflash_2_INFINITE'"));
	BulletParticle->GetParticleComponent()->AttachToComponent(NewBullet->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	NewBullet->OnDestory.AddUObject(BulletParticle, &AParticleNiagara::ParticleDestory);
	//NewBullet->OnDestory.AddUObject(this, &ACharacterBase::BlueShootCombo);
}

void ACharacterBase::Shoot()
{
	if (isShootLimitTime == true)
	{
		return;
	}

	isShootLimitTime = true;

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			isShootLimitTime = false;
		}
	), 0.5f, false);

	AnimInst->SetIsFire(true);

	FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	FRotator SpawnRotation = GetActorRotation();

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const FVector CameraStart = FollowCameraComponent->GetComponentLocation();
	const FVector CameraEnd = CameraStart + FollowCameraComponent->GetForwardVector() * 7000.f;

	FVector CollisionLocation = OutHitResult.Location;

	bool HitDetected = GetWorld()->LineTraceSingleByChannel(OutHitResult, CameraStart, CameraEnd, ECC_WorldStatic, Params);
	if (HitDetected)
	{
		CollisionLocation = OutHitResult.Location;
		//DrawDebugCapsule(GetWorld(), CollisionLocation, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);
	}

	else
	{
		CollisionLocation = FollowCameraComponent->GetForwardVector();
	}

	FVector TargetDirection = (CollisionLocation - MuzzleLocation).GetSafeNormal();
	SpawnRotation = TargetDirection.Rotation();


	SaveMuzzleLocation = MuzzleLocation;
	SaveSpawnRotation = SpawnRotation;

	if (CurrentCharacterType == ECharacterType::Blue)
	{
		if (Stat->SubBlueBullet(1) == false)
		{
			return;
		}

		CharacterShootings[(uint8)ECharacterType::Blue - 1].ShootingDelegate.ExecuteIfBound();
	}

	if (CurrentCharacterType == ECharacterType::Red)
	{
		if (Stat->SubRedBullet(1) == false)
		{
			return;
		}

		CharacterShootings[(uint8)ECharacterType::Red - 1].ShootingDelegate.ExecuteIfBound();
	}

	UE_LOG(LogCharacter, Log, TEXT("Shoot"));
}

void ACharacterBase::StopShoot()
{
	AnimInst->SetIsFire(false);
	HoldTimerHandle.Invalidate();
	
	if (isGreenShootingHold == true)
	{
		isGreenShootingHold = false;
	}

	if (GreenBulletHoldParticle)
	{
		GreenBulletHoldParticle->ParticleDestory();
	}

	isGreenShootingOne = true;

}

void ACharacterBase::Shooting()
{
	//UE_LOG(LogCharacter, Log, TEXT("Shooting"))

	//if (isShootLimitTime == true)
	//{
	//	return;
	//}
	//isShootLimitTime = true;
	//FTimerHandle ShootLimitTime;
	//GetWorld()->GetTimerManager().SetTimer(ShootLimitTime, FTimerDelegate::CreateLambda(
	//	[&]()
	//	{
	//		isShootLimitTime = false;
	//	}
	//), 1.0f, false);

	if (isGreenShootingOne == true)
	{
		isGreenShootingOne = false;
		AnimInst->SetIsFire(true);



		FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
		FRotator SpawnRotation = GetActorRotation();

		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		const FVector CameraStart = FollowCameraComponent->GetComponentLocation();
		const FVector CameraEnd = CameraStart + FollowCameraComponent->GetForwardVector() * 7000.f;

		FVector CollisionLocation = OutHitResult.Location;

		bool HitDetected = GetWorld()->LineTraceSingleByChannel(OutHitResult, CameraStart, CameraEnd, ECC_WorldStatic, Params);
		if (HitDetected)
		{
			CollisionLocation = OutHitResult.Location;
			//DrawDebugCapsule(GetWorld(), CollisionLocation, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);
		}

		else
		{
			CollisionLocation = FollowCameraComponent->GetForwardVector();
		}

		FVector TargetDirection = (CollisionLocation - MuzzleLocation).GetSafeNormal();
		SpawnRotation = TargetDirection.Rotation();


		SaveMuzzleLocation = MuzzleLocation;
		SaveSpawnRotation = SpawnRotation;

		//FOnBehaviorDelegate.Broadcast(ECharacterBehaviorType::Level3);
		CharacterShootings[(uint8)ECharacterType::Green - 1].ShootingDelegate.ExecuteIfBound();

	}
}

void ACharacterBase::SpectrumEffect()
{
	//if (isSpectrumCreateTime == true)
	//{
	//	return;
	//}
	//isSpectrumCreateTime = true;
	//GetWorld()->GetTimerManager().SetTimer(SpectrumCreateHandle, FTimerDelegate::CreateLambda(
	//	[&]()
	//	{
	//		isSpectrumCreateTime = false;
	//	}
	//), 0.1f, false);

	ASpectrumEffect* CharacterSpectrumEffect = GetWorld()->SpawnActor<ASpectrumEffect>(ASpectrumEffect::StaticClass(), GetActorLocation(), GetActorRotation());
	CharacterSpectrumEffect->Init(GetMesh());	
}

void ACharacterBase::Dash(const FInputActionValue& Value)
{
	UE_LOG(LogCharacter, Log, TEXT("Dash"));

	if (Stat->GetCurrentStemina() <= 2.0f)
	{
		AnimInst->SetIsDash(false);
		isDashSoundCheck = false;
		DeshTimerHandle.Invalidate();

		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		return;
	}

	if (isDashSoundCheck == false)
	{
		isDashSoundCheck = true;

		ASound* ADashSound = GetWorld()->SpawnActor<ASound>(ASound::StaticClass(), GetActorLocation(), GetActorRotation());
		ADashSound->SetSoundType(ESoundScale::Scale1, ESoundLevel::Level3);

		GetWorld()->GetTimerManager().SetTimer(DeshTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{
				isDashSoundCheck = false;
			}
		), 1.0f, false);
	}

	AnimInst->SetIsDash(true);
	Stat->ApplyStemina(0.1f);
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

	//if (isSpectrumTime == true)
	//{
	//	SpectrumEffect(InputMovementVector);
	//}
}

void ACharacterBase::ReleaseDash(const FInputActionValue& Value)
{
	AnimInst->SetIsDash(false);
	isDashSoundCheck = false;
	DeshTimerHandle.Invalidate();

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void ACharacterBase::ItemChangeUp()
{
	CurrentCharacterType = static_cast<ECharacterType>(static_cast<uint8>(CurrentCharacterType) + 1);
	if (CurrentCharacterType == ECharacterType::MAX)
	{
		CurrentCharacterType = static_cast<ECharacterType>(static_cast<uint8>(ECharacterType::MIN) + 1);
	}

	ChangeCharacterActions[(uint8)CurrentCharacterType - 1].CharacterChangeDelegate.ExecuteIfBound();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ItemChangeUp"));
}

void ACharacterBase::ItemChangeDown()
{
	CurrentCharacterType = static_cast<ECharacterType>(static_cast<uint8>(CurrentCharacterType) - 1);
	if (CurrentCharacterType == ECharacterType::MIN)
	{
		CurrentCharacterType = static_cast<ECharacterType>(static_cast<uint8>(ECharacterType::MAX) - 1);
	}
	//uint8 text = (uint8)CurrentCharacterType;
	ChangeCharacterActions[(uint8)CurrentCharacterType -1].CharacterChangeDelegate.ExecuteIfBound();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ItemChangeDown"));

}

void ACharacterBase::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();			//벡터 길이의 제곱을 계산한다. 
	float MovementVectorSize = 1.0f;									//벡터 초기화
	float MovementVectorSizeSquared = MovementVector.SquaredLength();	

	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void ACharacterBase::ReleaseSpectrumTime()
{

	//isSpectrumTime = false;
}

void ACharacterBase::SpectrumTime()
{
	if (isSpectrumTime == true || Stat->GetCurrentStemina() <= 9.f)
	{
		return;
	}

	//InPressedSpectrum = true;
	Stat->ApplyStemina(10.f);
	isSpectrumTime = true;
	GetMesh()->SetVisibility(false);
	SpectrumEffect();
	GetWorld()->GetTimerManager().SetTimer(SpectrumHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			isSpectrumTime = false;
			SpectrumEffect();

			GetMesh()->SetVisibility(true);
		}
	), 0.5f, false);

}

void ACharacterBase::Interaction()
{
	isInteraction = true;
}

void ACharacterBase::ReleaseInteraction()
{
	isInteraction = false;
}

void ACharacterBase::Light()
{
	isLight = !isLight;
	FlashLight->SetVisibility(isLight);
}

void ACharacterBase::Heal()
{
	if (IsHasHealKit)
	{
		Stat->RecoveryHp(10.f);
	}
}


void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ACharacterBase::Shoot);
		EnhancedInputComponent->BindAction(ReleaseShootAction, ETriggerEvent::Triggered, this, &ACharacterBase::StopShoot);
		EnhancedInputComponent->BindAction(ShootingAction, ETriggerEvent::Triggered, this, &ACharacterBase::Shooting);


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//ETriggerEvent::Ongoing
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::QuaterMove);
		EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ACharacterBase::ShoulderLook);

		EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::ShoulderMove);
		EnhancedInputComponent->BindAction(FPSLookAction, ETriggerEvent::Triggered, this, &ACharacterBase::FPSLook);
		EnhancedInputComponent->BindAction(FPSMoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::FPSMove);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACharacterBase::Dash);


		EnhancedInputComponent->BindAction(ReleaseDashAction, ETriggerEvent::Triggered, this, &ACharacterBase::ReleaseDash);

		EnhancedInputComponent->BindAction(ViewChangeAction, ETriggerEvent::Triggered, this, &ACharacterBase::ChangeCharacterControl);
		EnhancedInputComponent->BindAction(WheelUpAction, ETriggerEvent::Triggered, this, &ACharacterBase::ItemChangeUp);
		EnhancedInputComponent->BindAction(WheelDownAction, ETriggerEvent::Triggered, this, &ACharacterBase::ItemChangeDown);
	
		EnhancedInputComponent->BindAction(SpectrumAction, ETriggerEvent::Triggered, this, &ACharacterBase::SpectrumTime);
		EnhancedInputComponent->BindAction(RaleaseSpectrumAction, ETriggerEvent::Triggered, this, &ACharacterBase::ReleaseSpectrumTime);
	
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &ACharacterBase::Interaction);
		EnhancedInputComponent->BindAction(ReleaseInteractionAction, ETriggerEvent::Triggered, this, &ACharacterBase::ReleaseInteraction);


		EnhancedInputComponent->BindAction(LightAction, ETriggerEvent::Triggered, this, &ACharacterBase::Light);
		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &ACharacterBase::Heal);

	}
}

void ACharacterBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ECollisionChannel ActorCollisionChannel = OtherComp->GetCollisionObjectType();
	if (ActorCollisionChannel == CCHANNEL_MONSTEROBJECT && isSpectrumTime == false)
	{
		AnimInst->Montage_Play(HitMontage);
		FDamageEvent DamageEvent;
		TakeDamage(10.f, DamageEvent, nullptr, this);
		AnimInst->SetIsHit(false);
		UE_LOG(LogCharacter, Log, TEXT("CollMonster"));
	}
}

void ACharacterBase::isPossibelAttack(float _DeltaTime)
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const FVector Start = MuzzleLocation;

	const FVector CameraStart = FollowCameraComponent->GetComponentLocation();
	const FVector CameraEnd = CameraStart + FollowCameraComponent->GetForwardVector() * 5000.f;

	bool HitDetected = GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, CameraEnd, CCHANNEL_ACTION, Params);
	if (HitDetected)
	{
		FVector CollisionLocation = OutHitResult.Location;
		//DrawDebugCapsule(GetWorld(), CollisionLocation, 50.f, 50.f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), FColor::Green, false, 5.0f);
		
		UI_MainHUD->AddSightWidgetTransform(50.f , _DeltaTime);
		UI_MainHUD->SetbTargeting(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AttackPossible"));
	}
	else
	{
		UI_MainHUD->SetbTargeting(false);
	}
}

void ACharacterBase::LightVsMonster(float _DeltaTime)
{
	if (LightTimer == true || isLight == false)
	{
		return;
	}

	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const FVector Start = FlashLight->GetComponentLocation();

	const FVector LightStart = Start;
	const FVector LightEnd = LightStart + FlashLight->GetForwardVector() * 700.f;

	bool HitDetected = GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, LightEnd, CCHANNEL_ACTION, Params);
	if (HitDetected)
	{
		AFloatRobot* Monster = Cast<AFloatRobot>(OutHitResult.GetActor());
		if (Monster)
		{
			Monster->ChangeStimulationLevel(ESoundLevel::Level2);
			Monster->OnAIUpdateDetectLevel();
			LightTimer = true;
			GetWorld()->GetTimerManager().SetTimer(LightTimerHandle, FTimerDelegate::CreateLambda(
				[&]()
				{
					LightTimer = false;
				}
			), 2.0f, false);
		}
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AnimInst->GetIsHit() == false)
	{
		AnimInst->SetIsHit(true);
	}
	//카메라 쉐이크
	//AFPSPlayerController* FPSPlayerController = CastChecked<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());
	//TSubclassOf<UCameraShakeBase> PlayerCameraShakeClass = UCameraShakeBase::StaticClass();
	FPSPlayerController->PlayerCameraManager->StartCameraShake(UCharacterCameraShake::StaticClass(), 3.0f);

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Take Damage"));
	Stat->ApplyDamage(DamageAmount);
	return 0.0f;
}

void ACharacterBase::SetupCharacterWidget(UMasterWidget* InUserWidget)
{
	UHPWidget* HPWidget = Cast<UHPWidget>(InUserWidget);
	if (HPWidget)
	{
		HPWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HPWidget->SetHp(Stat->GetTotalStat().MaxHp);
		Stat->OnHpChanged.AddUObject(HPWidget, &UHPWidget::SetHp); //Broadcast 가 될떄 불러와진다.
	}

	UStaminaWidget* SteminaWidget = Cast<UStaminaWidget>(InUserWidget);
	if (SteminaWidget)
	{
		SteminaWidget->SetMaxStemina(Stat->GetTotalStat().Stemina);
		SteminaWidget->SetStemina(Stat->GetTotalStat().Stemina);
		Stat->OnSteminaChanged.AddUObject(SteminaWidget, &UStaminaWidget::SetStemina); //Broadcast 가 될떄 불러와진다.
	}

	//UHeartWidget* HeartWidget = Cast<UHeartWidget>(InUserWidget);
	//if (HeartWidget)
	//{
	//	HeartWidget->SetMaxHp(Stat->GetMaxHP());
	//	HeartWidget->SetHp(Stat->GetCurrentHp());
	//	//Stat->OnHpChanged.AddUObject(HpBarWidget, &UABHpBarWidget::UpdateHpBar);
	//}
}

bool ACharacterBase::GetIsInteractionAction()
{
	return isInteraction;
}

void ACharacterBase::TakeItem(UItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}


void ACharacterBase::TakeHealKit(UItemData* InItemData)
{
	IsHasHealKit = true;
	UI_MainHUD->GetGuideWidget()->OnGuideTextChange(FString("got the HealKit"), FString("refilled the HealKit."), FString(" "));
}

void ACharacterBase::TakeBlueBullet(UItemData* InItemData)
{
	Stat->AddBlueBullet(InItemData->Count);
	UI_MainHUD->GetGuideWidget()->OnGuideTextChange(FString("got the BlueBullet"), FString("refilled the bullets."), FString(" "));
}

void ACharacterBase::TakeGreenBullet(UItemData* InItemData)
{
	Stat->AddGreenBullet(InItemData->Count);
	UI_MainHUD->GetGuideWidget()->OnGuideTextChange(FString("got the GreenBullet"), FString("refilled the bullets."), FString(" "));
}

void ACharacterBase::TakeRedBullet(UItemData* InItemData)
{
	Stat->AddRedBullet(InItemData->Count);
	UI_MainHUD->GetGuideWidget()->OnGuideTextChange(FString("got the RedBullet"), FString("refilled the bullets."), FString(" "));
}

void ACharacterBase::TakeKey(class UItemData* InItemData)
{
	UI_MainHUD->GetGuideWidget()->OnGuideTextChange(FString("got the key"), FString("won't be any more difficulties on the way to Target."), FString("Are there any obstacles?"));

}