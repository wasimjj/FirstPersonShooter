// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TeamFlagBase.generated.h"

UCLASS()
class CTFTASK_API ATeamFlagBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category="Flag" )
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category="Flag" )
	UStaticMeshComponent* FlagMesh;
	// Sets default values for this actor's properties
	ATeamFlagBase();
	UFUNCTION(BlueprintCallable , Category="Overlap")
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly , Category="Flag")
	bool bIsFlagOnTheBase = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category="Flag")
	bool bIsBlueFlagBase = true;


};
