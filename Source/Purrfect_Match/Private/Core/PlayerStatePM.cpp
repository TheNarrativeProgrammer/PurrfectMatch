// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerStatePM.h"

#include "Core/GameStatePM.h"
#include "Logging/StructuredLog.h"
#include "Kismet/GameplayStatics.h"

APlayerStatePM::APlayerStatePM()
{
}

void APlayerStatePM::ChangeScore(float ScoreIncrumentAmount)
{
	float oldScore = GetScore();
	float newScore = ScoreIncrumentAmount + oldScore;
	SetScore(newScore);

	if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(UGameplayStatics::GetGameState(GetWorld())))
	{
		UE_LOGFMT(LogTemp, Warning, "oldScore {0}", oldScore);
		UE_LOGFMT(LogTemp, Warning, "newScore {0}", newScore);
		UE_LOGFMT(LogTemp, Warning, "ScoreIncrumentAmount {0}", ScoreIncrumentAmount);
		GameStatePM->PlayerStateScoreChangeDelegate.Broadcast(oldScore, newScore,ScoreIncrumentAmount);
	}
}

void APlayerStatePM::ChangePlayerLivesRemaining(int32 AmountLivesGainedOrLost)
{
	int32 oldLivesRemaining = playerLivesRemaining;
	playerLivesRemaining += AmountLivesGainedOrLost;

	if (AGameStatePM* GameStatePM = Cast<AGameStatePM>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameStatePM->PlayerStateLiveChangeDelegate.Broadcast(oldLivesRemaining, playerLivesRemaining, AmountLivesGainedOrLost);
	}
}


