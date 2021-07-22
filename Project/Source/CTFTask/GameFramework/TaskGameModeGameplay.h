// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "TaskGameModeGameplay.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlagCapturedDelegate,bool, bIsBlueTeam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotifyDelegate);
UCLASS()
class CTFTASK_API ATaskGameModeGameplay : public ATaskGameMode
{
	GENERATED_BODY()
	protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
public:
	ATaskGameModeGameplay();
	virtual void BeginPlay() override;
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void SpawnTempTeamBase(const bool bIsBlue,FVector SpawnLocation);
	UFUNCTION()
	ATeamFlagBase* SpawnFlagBase(const bool bIsBlueBase,bool bIsPermanent ,FVector SpawnLocation);
	UFUNCTION()
	void OnFlagCaptured(bool bIsBlueTeam);
	public:
	UPROPERTY(EditDefaultsOnly, Category="Team Base")
	TSubclassOf<ATeamFlagBase> BlueTeamFlagBasePreset;
	UPROPERTY(EditDefaultsOnly, Category="Team Base")
	TSubclassOf<ATeamFlagBase> RedTeamFlagBasePreset;
	UPROPERTY(BlueprintReadOnly, Category="Team Base")
	ATeamFlagBase* BlueTeamFlagBase;
	UPROPERTY(BlueprintReadOnly, Category="Team Base")
	ATeamFlagBase* RedTeamFlagBase;
	UPROPERTY(BlueprintReadOnly, Category="Team Base")
	ATeamFlagBase* TempBlueTeamFlagBase;
	UPROPERTY(BlueprintReadOnly, Category="Team Base")
	ATeamFlagBase* TempRedTeamFlagBase;
	UPROPERTY()
	FOnFlagCapturedDelegate OnFlagCapturedDelegate;
	UPROPERTY(BlueprintAssignable , Category="Game Start")
	FNotifyDelegate OnGameStartDelegate;
	UPROPERTY(BlueprintReadOnly , Category="Players")
	int PlayersInGame;
	UPROPERTY(BlueprintReadOnly , Category="Rule")
	int MaxPlayer = 2;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Rule")
	int GameMatchTime = 300;
};
