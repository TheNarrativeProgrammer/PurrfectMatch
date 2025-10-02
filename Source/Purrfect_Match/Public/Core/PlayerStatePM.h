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

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Lives")
	int32 playerLivesRemaining = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Lives")
	int32 playerLivesDefaultAmount = 9;

	

	UFUNCTION()
	void ResetLivesToDefault();



public:

	UFUNCTION()
	void ResetScoreToZero();
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void ChangeScore(float ScoreIncrumentAmount);

	UFUNCTION(BlueprintCallable, Category = "Player Lives")
	int32 GetPlayerLivesRemaining() const {return playerLivesRemaining;}

	UFUNCTION(BlueprintCallable, Category = "Player Lives")
	void ChangePlayerLivesRemaining(int32 AmountLivesGainedOrLost);
	

	
};







