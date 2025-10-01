// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameStatePM.h"

#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"


void AGameStatePM::BeginPlay()
{
	Super::BeginPlay();
}



void AGameStatePM::HandleRestartingLevel()
{
	GameStateRestartLevelDelegate.Broadcast();
}

void AGameStatePM::HandleRestartingGame()
{
	if (AGameMode* GameMode = Cast<AGameMode>( UGameplayStatics::GetGameMode(GetWorld())))
	{
		FName MatchStateStatus1 = GameMode->GetMatchState();
		GameMode->RestartGame();
		FName MatchStateStatus2 = GameMode->GetMatchState();
		// GameMode->StartMatch();
		// FName MatchStateStatus3 = GameMode->GetMatchState();
		// FName MatchStateStatus = GameMode->GetMatchState();
	}
	//GameStateRestartGameDelegate.Broadcast();
}

void AGameStatePM::HandleMatchHasEnded()
{
	if (AGameMode* GameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		FName MatchStateStatus2 = GameMode->GetMatchState();
		Super::HandleMatchHasEnded();
		GameStateMatchEndDelegate.Broadcast();
		FName MatchStateStatus3 = GameMode->GetMatchState();
	}
	// Super::HandleMatchHasEnded();
	// GameStateMatchEndDelegate.Broadcast();
	
}
