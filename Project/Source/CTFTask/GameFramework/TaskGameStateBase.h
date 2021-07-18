// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TaskGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CTFTASK_API ATaskGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(ReplicatedUsing=OnRep_OnBlueTeamScore , BlueprintReadOnly , Category="Score")
	int BlueTeamScore;
	UPROPERTY(ReplicatedUsing=OnRep_OnRedTeamScore , BlueprintReadOnly , Category="Score")
	int RedTeamScore;
	UFUNCTION(BlueprintCallable)
	void OnRep_OnBlueTeamScore();
	UFUNCTION(BlueprintCallable)
	void OnRep_OnRedTeamScore();
};
