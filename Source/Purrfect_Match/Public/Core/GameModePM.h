// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModePM.generated.h"

/**
 * 
 */
class APlayerStart;
class APlayerSpawnLocation;

UCLASS()
class PURRFECT_MATCH_API AGameModePM : public AGameMode
{
	GENERATED_BODY()


	public:
	AGameModePM();
	
	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer) override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	TSubclassOf<APlayerSpawnLocation> SpawnPointClass;
	
};
