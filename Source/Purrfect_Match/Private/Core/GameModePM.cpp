// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModePM.h"
#include "Core/GameStatePM.h"
#include "Core/PlayerControllerPM.h"
#include "Pawns/PawnPlayerPM.h"


AGameModePM::AGameModePM()
{
	GameStateClass = AGameStatePM::StaticClass();
	PlayerControllerClass = APlayerControllerPM::StaticClass();
	DefaultPawnClass = APawnPlayerPM::StaticClass();
}

void AGameModePM::BeginPlay()
{
	Super::BeginPlay();
}
