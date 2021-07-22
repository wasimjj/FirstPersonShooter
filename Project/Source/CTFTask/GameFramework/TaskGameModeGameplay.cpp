// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskGameModeGameplay.h"
#include "TaskHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void ATaskGameModeGameplay::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayersInGame++;
	if (PlayersInGame >= MaxPlayer)
	{
		StartGame();
	}
}

ATaskGameModeGameplay::ATaskGameModeGameplay()
{
	HUDClass = ATaskHUD::StaticClass();
}

void ATaskGameModeGameplay::BeginPlay()
{
	OnFlagCapturedDelegate.AddDynamic(this, &ATaskGameModeGameplay::OnFlagCaptured);
}

void ATaskGameModeGameplay::StartGame()
{
	BlueTeamFlagBase = SpawnFlagBase(true, true, FVector(-1700.0f, 1400.0f, 270.0f));
	RedTeamFlagBase = SpawnFlagBase(false, true, FVector(30.0f, -1400.0f, 270.0f));
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	OnGameStartDelegate.Broadcast();
}

void ATaskGameModeGameplay::SpawnTempTeamBase(const bool bIsBlue, FVector SpawnLocation)
{
	if (bIsBlue)
	{
		if (TempBlueTeamFlagBase == nullptr)
		{
			TempBlueTeamFlagBase = SpawnFlagBase(bIsBlue, false, SpawnLocation);
			TempBlueTeamFlagBase->bIsFlagOnTheBase = true;
			return;
		}

		TempBlueTeamFlagBase->SetActorHiddenInGame(false);
		TempBlueTeamFlagBase->BoxComponent->SetGenerateOverlapEvents(true);
		TempBlueTeamFlagBase->SetActorLocation(SpawnLocation);
		return;
	}

	if (TempRedTeamFlagBase == nullptr)
	{
		TempRedTeamFlagBase = SpawnFlagBase(bIsBlue, false, SpawnLocation);
		TempRedTeamFlagBase->bIsFlagOnTheBase = true;
		return;
	}

	TempRedTeamFlagBase->SetActorHiddenInGame(false);
	TempRedTeamFlagBase->BoxComponent->SetGenerateOverlapEvents(true);
	TempRedTeamFlagBase->SetActorLocation(SpawnLocation);
}

ATeamFlagBase* ATaskGameModeGameplay::SpawnFlagBase(const bool bIsBlueBase, bool bIsPermanent, FVector SpawnLocation)
{
	ATeamFlagBase* TeamFlagBase;
	SpawnLocation.Z = 270.0f;
	if (bIsBlueBase)
	{
		TeamFlagBase = GetWorld()->SpawnActor<ATeamFlagBase>(BlueTeamFlagBasePreset, SpawnLocation,
		                                                     FRotator::ZeroRotator);
	}
	else
	{
		TeamFlagBase = GetWorld()->SpawnActor<ATeamFlagBase>(RedTeamFlagBasePreset, SpawnLocation,
		                                                     FRotator::ZeroRotator);
	}
	TeamFlagBase->bIsPermanent = bIsPermanent;
	TeamFlagBase->bIsFlagOnTheBase = true;
	TeamFlagBase->bIsBlueTeamBase = bIsBlueBase;
	return TeamFlagBase;
}

void ATaskGameModeGameplay::OnFlagCaptured(bool bIsBlueTeam)
{
	if (bIsBlueTeam)
	{
		RedTeamFlagBase->ResetFlagBase();
		return;
	}
	BlueTeamFlagBase->ResetFlagBase();
}
