// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamFlagBase.h"

#include "TaskCharacter.h"
#include "TaskGameModeGameplay.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATeamFlagBase::ATeamFlagBase()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TeamsFlag"));
	BoxComponent->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	BoxComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionProfileName("Custom");

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetCollisionProfileName("Custom");
	FlagMesh->SetupAttachment(BoxComponent);
}

void ATeamFlagBase::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (PlayerController->GetLocalRole() == ROLE_Authority)
		{
			BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeamFlagBase::OnComponentBeginOverlap);
		}
	}
	TaskGameModeGameplay = Cast<ATaskGameModeGameplay>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ATeamFlagBase::ResetFlagBase()
{
	FlagMesh->SetVisibility(true);
	bIsFlagOnTheBase = true;
}

void ATeamFlagBase::OnCharacterOverlap(ACTFTaskCharacter* CTFTaskCharacter)
{
	if (bIsPermanent)
	{
		SetFlagAvailability(!(CTFTaskCharacter->TaskPlayerState->bIsTeamBlue != bIsBlueTeamBase));
		CTFTaskCharacter->SetFlagVisibility(bIsBlueTeamBase);
		return;
	}
	SetActorHiddenInGame(true);
	BoxComponent->SetGenerateOverlapEvents(false);
	if (CTFTaskCharacter->TaskPlayerState->bIsTeamBlue != bIsBlueTeamBase)
	{
		CTFTaskCharacter->SetFlagVisibility(bIsBlueTeamBase);
		return;
	}
	
	if (bIsBlueTeamBase)
	{
		TaskGameModeGameplay->BlueTeamFlagBase->SetFlagAvailability(true);
		return;
	}
	
	TaskGameModeGameplay->RedTeamFlagBase->SetFlagAvailability(true);
}

void ATeamFlagBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && bIsFlagOnTheBase)
	{
		if (ACTFTaskCharacter* CTFTaskCharacter = Cast<ACTFTaskCharacter>(OtherActor))
		{
			if (CTFTaskCharacter->TaskPlayerState->CurrentHealth > 0)
			{
				OnCharacterOverlap(CTFTaskCharacter);
			}
		}
	}
}

void ATeamFlagBase::SetFlagAvailability(const bool bIsAvailable)
{
	FlagMesh->SetVisibility(bIsAvailable);
	bIsFlagOnTheBase = bIsAvailable;
}
