// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerStatePM.h"

APlayerStatePM::APlayerStatePM()
{
}

void APlayerStatePM::IncreaseScore(float ScoreIncrumentAmount)
{
	float newScore = ScoreIncrumentAmount + GetScore();
	SetScore(newScore);
	FString scoreString = FString::Printf(TEXT("Score Increase: %f"), newScore);
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Blue, scoreString);
}

void APlayerStatePM::DecreaseScore(float ScoreDecrumentAmount)
{
	float newScore = ScoreDecrumentAmount - GetScore();
	SetScore(newScore);
	FString scoreString = FString::Printf(TEXT("Score decrease: %f"), newScore);
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, scoreString);
}
