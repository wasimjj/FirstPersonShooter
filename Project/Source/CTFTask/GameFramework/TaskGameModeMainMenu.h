// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskGameMode.h"
#include "Interfaces/OnlineSessionInterface.h" 
#include "TaskGameModeMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class CTFTASK_API ATaskGameModeMainMenu : public ATaskGameMode
{
	GENERATED_BODY()
public:
	ATaskGameModeMainMenu();
	IOnlineSessionPtr OnlineSessionPtr;

	virtual void OnSessionComplete(FName ServerName, bool Success);
	UFUNCTION(BlueprintCallable,Category= "Session")
	void CreateServer();
	UFUNCTION(BlueprintCallable , Category="Session")
	void JoinServer();
};
