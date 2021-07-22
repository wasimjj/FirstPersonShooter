// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TaskPlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreUpdateDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCaptureFlagStatusUpdateDelegate, bool, bIsCapture);


UCLASS()
class CTFTASK_API ATaskPlayerState : public APlayerState
{
	GENERATED_BODY()
	protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
public:
	UPROPERTY(ReplicatedUsing=OnRep_IsTeamBlue ,BlueprintReadOnly, Category="Player State")
	bool bIsTeamBlue;
	UFUNCTION()
	void OnRep_IsTeamBlue();

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100;
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing=OnRep_IsFlagCaptured)
	bool bIsFlagCaptured;
	UFUNCTION()
	void OnRep_IsFlagCaptured();
	UFUNCTION()
	void OnRep_CurrentHealth();
	void OnHealthUpdate();
	UPROPERTY(BlueprintAssignable , Category="Score")
	FOnScoreUpdateDelegate OnScoreUpdateDelegate;
	UPROPERTY(BlueprintAssignable, Category="Flag")
	FCaptureFlagStatusUpdateDelegate CaptureFlagStatusUpdateDelegate;
	
};
