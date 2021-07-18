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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category="Flag")
	bool bIsBlueTeamBase = true;
	//UFUNCTION(Server , Reliable ,Category="Falg")
	//void PlayerFlagChangeVisibility(ACTFTaskCharacter* TaskCharacter,const bool bIsVisibility);
	public:
	UPROPERTY(/*ReplicatedUsing=OnRep_UpdateFlagOnTheBase, BlueprintReadOnly , Category="Flag"*/)
	bool bIsFlagOnTheBase = true;
	//UFUNCTION()
	//void OnRep_UpdateFlagOnTheBase();
};
