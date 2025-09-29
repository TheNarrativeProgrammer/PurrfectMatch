// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStatePM.generated.h"

/**
 * 
 */
UCLASS()
class PURRFECT_MATCH_API APlayerStatePM : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerStatePM();


public:
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void IncreaseScore(float ScoreIncrumentAmount);

	UFUNCTION(BlueprintCallable, Category = "Score")
	void DecreaseScore(float ScoreDecrumentAmount);

	
};







