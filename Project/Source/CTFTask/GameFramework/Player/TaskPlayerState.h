// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TaskPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CTFTASK_API ATaskPlayerState : public APlayerState
{
	GENERATED_BODY()
	protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(ReplicatedUsing=OnRep_IsTeamBlue ,BlueprintReadOnly, Category="Player State")
	bool bIsTeamBlue;
	UFUNCTION()
	void OnRep_IsTeamBlue();
 	
};
