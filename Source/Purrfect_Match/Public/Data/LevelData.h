// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LevelData.generated.h"

/**
 * 
 */
UCLASS()
class PURRFECT_MATCH_API ULevelData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	float TimeToSpawnNewRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	FGameplayTag GameplayTagGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Data")
	int32 AffectionLevelMax;

	
	
};
