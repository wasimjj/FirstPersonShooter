// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TaskCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TeamFlagBase.generated.h"

UCLASS()
class CTFTASK_API ATeamFlagBase : public AActor
{
	GENERATED_BODY()

public:
	ATeamFlagBase();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flag")
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flag")
	UStaticMeshComponent* FlagMesh;
	UPROPERTY()
	class ATaskGameModeGameplay* TaskGameModeGameplay;
	UFUNCTION(BlueprintCallable, Category="Overlap")
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	void SetFlagAvailability(bool bIsAvailable);
	UFUNCTION()
	void ResetFlagBase();
	UFUNCTION()
	void OnCharacterOverlap(ACTFTaskCharacter* CTFTaskCharacter);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flag")
	bool bIsBlueTeamBase = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flag")
	bool bIsFlagOnTheBase = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Flag")
	bool bIsPermanent;
};
