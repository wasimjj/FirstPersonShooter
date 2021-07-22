// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TaskGameStateBase.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartDelegate);

UENUM(BlueprintType)
enum class EGameState : uint8
{
	EGameLobby 	UMETA(DisplayName = "Game Lobby"),
	EGamePlay 	UMETA(DisplayName = "Game Play"),
	EGameEnd 	UMETA(DisplayName = "Game End"),
};
UCLASS()
class CTFTASK_API ATaskGameStateBase : public AGameState
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	class ATaskGameModeGameplay* TaskGameModeGameplay;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnGameStart();
	UFUNCTION()
	void UpdateGameTimer();
	UFUNCTION()
	void CheckIfTimerEnd();
public:
	
	//UPROPERTY()
	//bool bIsBlueTeam;
	UPROPERTY(ReplicatedUsing=OnRep_OnBlueTeamScore , BlueprintReadOnly , Category="Score")
	int BlueTeamScore;
	UPROPERTY(ReplicatedUsing=OnRep_OnRedTeamScore , BlueprintReadOnly , Category="Score")
	int RedTeamScore;
	UPROPERTY(ReplicatedUsing=OnRep_OnGameTimerUpdate , BlueprintReadOnly , Category="Timer")
	int GameMatchTimer;
	UPROPERTY(ReplicatedUsing=OnRep_OnChangeGameStateType)
	EGameState GameStateType;
	UPROPERTY(BlueprintAssignable)
	FOnGameStartDelegate OnGameStartDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnGameStartDelegate OnGameEndDelegate;
	UFUNCTION()
	void OnRep_OnChangeGameStateType();
	UFUNCTION(BlueprintCallable)
	void OnRep_OnBlueTeamScore();
	
	UFUNCTION(BlueprintCallable)
	void OnRep_OnRedTeamScore();
	UFUNCTION()
	void OnRep_OnGameTimerUpdate();
	UFUNCTION()
	void OnGameStateChange();
	UFUNCTION()
	void UpdateGameState(EGameState GameState);
	UFUNCTION()
	void AddScore(const bool bIsBlueTeam);
	
	
};
