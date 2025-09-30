// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameStatePM.h"

void AGameStatePM::BeginPlay()
{
	Super::BeginPlay();
}



void AGameStatePM::HandleRestartingLevel()
{
	GameStateRestartLevelDelegate.Broadcast();
}

void AGameStatePM::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	GameStateMatchEndDelegate.Broadcast();
}
