// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameStatePM.h"

void AGameStatePM::SetLevelStage(TEnumAsByte<ELevelStage> LevelStage)
{
	CurrentLevelStage = LevelStage;
}

void AGameStatePM::HandleRestartingLevel()
{
	GameStateLiveLostRestartLevelDelegate.Broadcast();
}
