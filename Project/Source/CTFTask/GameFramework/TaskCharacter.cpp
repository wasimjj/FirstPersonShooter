// Copyright Epic Games, Inc. All Rights Reserved.

#include "TaskCharacter.h"
#include "TaskProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "TaskHUD.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "CTFTask/Engine/TaskGameInstance.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACTFTaskCharacter

ACTFTaskCharacter::ACTFTaskCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	//Create mesh body for client
	MeshCharacterBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCharacterBody"));
	MeshCharacterBody->SetOnlyOwnerSee(false);
	MeshCharacterBody->SetOwnerNoSee(true);
	MeshCharacterBody->CastShadow = false;
	MeshCharacterBody->SetupAttachment(RootComponent);
	MeshCharacterBody->SetRelativeRotation(FRotator(0, -90.0f, 0));
	MeshCharacterBody->SetRelativeLocation(FVector(0, 0, -90));
	//Create mesh for flag
	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	FlagMesh->SetupAttachment(RootComponent);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetOnlyOwnerSee(false);
	FlagMesh->SetOwnerNoSee(true);
	FlagMesh->CastShadow = false;
	FlagMesh->SetVisibility(false);
	// Create a gun mesh component
	ClientBodyGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClientBodyGun"));
	ClientBodyGun->SetOnlyOwnerSee(false);
	ClientBodyGun->SetOwnerNoSee(true);
	ClientBodyGun->bCastDynamicShadow = false;
	ClientBodyGun->CastShadow = false;
	ClientBodyGun->SetupAttachment(MeshCharacterBody);
	ClientBodyGun->SetRelativeRotation(FRotator(0, 0, 0));
	ClientBodyGun->SetRelativeLocation(FVector(0, 0, 0));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true); // only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true); // only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f)); // Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
	//
	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

void ACTFTaskCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if (!IsLocallyControlled()) // if remote player 
	{
		ClientBodyGun->AttachToComponent(MeshCharacterBody,
		                                 FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		                                 TEXT("GunSocket"));
	}
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
	                          TEXT("GripPoint"));


	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	TimerDel.BindUFunction(this, FName("PlayerStateSetup"), 0, 0.0);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, .2f, false);
}

void ACTFTaskCharacter::PlayerStateSetup()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		TaskPlayerState = PlayerController->GetPlayerState<ATaskPlayerState>();
	}
	UTaskGameInstance* TaskGameInstance = Cast<UTaskGameInstance>(GetWorld()->GetGameInstance());
	if (IsLocallyControlled())
	{
		PlayerStateSetupInternal(TaskGameInstance->PlayerDataStruct);
		UE_LOG(LogTemp, Warning, TEXT("IsLocallyControlled :::"));
	}
}

void ACTFTaskCharacter::PlayerStateSetupInternal_Implementation(FPlayerDataStruct PlayerDataStruct)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerController found :::"));
	if (TaskPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("TaskPlayerState found :::"));

		TaskPlayerState->SetPlayerName(PlayerDataStruct.PlayerName);
		TaskPlayerState->bIsTeamBlue = PlayerDataStruct.bIsBlueTeam;
		UE_LOG(LogTemp, Warning, TEXT("My name is :::%s ::: my color :::%d"), *TaskPlayerState->GetPlayerName(),
		       TaskPlayerState->bIsTeamBlue);
		// if(ATaskHUD* TaskHUD = Cast<ATaskHUD>(PlayerController->GetHUD()))
		// {
		// 	TaskHUD->TaskPlayerStateRed = TaskPlayerState;
		// 	if(TaskPlayerState->bIsTeamBlue)
		// 	{
		// 		TaskHUD->TaskPlayerStateBlue = TaskPlayerState;
		// 	}
		// 	if(TaskHUD->TaskPlayerStateRed != nullptr && TaskHUD->TaskPlayerStateBlue != nullptr)
		// 	{
		// 		TaskHUD->OnRedTeamInitializeDelegate.Broadcast();
		// 	}
		// 	
		// }
	}
}


void ACTFTaskCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//if(IsLocallyControlled())
	// {
	// 	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	// 	{
	// 		TaskPlayerState = PlayerController->GetPlayerState<ATaskPlayerState>();
	// 		UE_LOG(LogTemp,Warning,TEXT("PlayerController found :::"));
	//
	// 		if(TaskPlayerState )
	// 		{
	// 			UE_LOG(LogTemp,Warning,TEXT("TaskPlayerState found :::"));
	// 			if (UTaskGameInstance* TaskGameInstance = Cast<UTaskGameInstance>(GetWorld()->GetGameInstance()))
	// 			{
	// 				TaskPlayerState->SetPlayerName(TaskGameInstance->PlayerDataStruct.PlayerName);
	// 				TaskPlayerState->bIsTeamBlue = TaskGameInstance->PlayerDataStruct.bIsBlueTeam;
	// 				UE_LOG(LogTemp,Warning,TEXT("My name is :::%s ::: my color :::%d"),*TaskPlayerState->GetPlayerName(),TaskPlayerState->bIsTeamBlue);
	// 			}
	// 		}
	// 	}
	// }
}


float ACTFTaskCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "TakeDamage from c++");
	SetCurrentHealth(CurrentHealth - DamageAmount);
	return DamageAmount;
}

void ACTFTaskCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACTFTaskCharacter, CurrentHealth);
}


//////////////////////////////////////////////////////////////////////////
// Input

void ACTFTaskCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACTFTaskCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACTFTaskCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACTFTaskCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACTFTaskCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ACTFTaskCharacter::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ACTFTaskCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ACTFTaskCharacter::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &ACTFTaskCharacter::LookUpAtRate);
}

void ACTFTaskCharacter::AddControllerPitchInput(float Val)
{
	if (Val != 0.f)
	{
		Super::AddControllerPitchInput(Val);
		if (IsLocallyControlled())
		{
			if (GetLocalRole() == ROLE_Authority)
			{
				CorrectRotationMulticast(GetControlRotation());
			}
			else
			{
				CorrectRotationOnServer(GetControlRotation());
			}
		}
	}
}

void ACTFTaskCharacter::AddControllerYawInput(float Val)
{
	if (Val != 0.f)
	{
		Super::AddControllerYawInput(Val);
	}
}


void ACTFTaskCharacter::OnFire()
{
	// if(TaskPlayerState != nullptr)
	// {
	// 	UE_LOG(LogTemp,Warning,TEXT("My Name : %s ::: Blue team:::%d"),*TaskPlayerState->GetPlayerName(),TaskPlayerState->bIsTeamBlue);
	// }
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				GetWorld()->SpawnActor<ACTFTaskProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr)
					                               ? FP_MuzzleLocation->GetComponentLocation()
					                               : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				OnFireServer(SpawnLocation, SpawnRotation);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ACTFTaskCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACTFTaskCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ACTFTaskCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.

void ACTFTaskCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ACTFTaskCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ACTFTaskCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACTFTaskCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ACTFTaskCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ACTFTaskCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ACTFTaskCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ACTFTaskCharacter::TouchUpdate);
		return true;
	}

	return false;
}

void ACTFTaskCharacter::OnFireServer_Implementation(const FVector Location, const FRotator Rotation)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.Instigator = GetInstigator();
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<ACTFTaskProjectile>(ProjectileClass, Location, Rotation, ActorSpawnParams);
}

void ACTFTaskCharacter::CorrectRotationOnServer_Implementation(FRotator Rotator)
{
	CharacterRotationCorrection = Rotator;
	if (!IsLocallyControlled())
	{
		FirstPersonCameraComponent->SetWorldRotation(Rotator);
	}
}

void ACTFTaskCharacter::CorrectRotationMulticast_Implementation(FRotator Rotator)
{
	CharacterRotationCorrection = Rotator;
	if (!IsLocallyControlled())
	{
		FirstPersonCameraComponent->SetWorldRotation(Rotator);
	}
}

void ACTFTaskCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ACTFTaskCharacter::OnHealthUpdate()
{
	if (IsLocallyControlled())
	{
		const FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			const FString DeathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DeathMessage);
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		const FString HealthMessage = FString::Printf(
			TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HealthMessage);
	}
}

void ACTFTaskCharacter::SetCurrentHealth(float HealthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(HealthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

void ACTFTaskCharacter::SetFlagVisibility(const bool bIsBaseBlue)
{
	if( TaskPlayerState!=nullptr)
	{
		UE_LOG(LogTemp,Warning, TEXT("ACTFTaskCharacter::SetFlagVisibility score:::%d ::::name::%s"),TaskPlayerState->GetScore(),*TaskPlayerState->GetPlayerName());
		if (bIsFlagCaptured && bIsBaseBlue == TaskPlayerState->bIsTeamBlue) // Flag Captured and reached to his own base
			{
			TaskPlayerState->SetScore(TaskPlayerState->GetScore() + 1);
			UE_LOG(LogTemp,Warning, TEXT("ACTFTaskCharacter::SetFlagVisibility score:::%d ::::name::%s"),TaskPlayerState->GetScore(),*TaskPlayerState->GetPlayerName());
			}
		bIsFlagCaptured = bIsBaseBlue != TaskPlayerState->bIsTeamBlue;
		FlagMesh->SetVisibility(bIsBaseBlue != TaskPlayerState->bIsTeamBlue);
		CaptureFlagStatusUpdateDelegate.Broadcast(bIsFlagCaptured);
	}
	
}
