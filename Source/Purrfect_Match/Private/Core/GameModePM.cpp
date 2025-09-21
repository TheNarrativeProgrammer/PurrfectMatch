// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModePM.h"
#include "Core/GameStatePM.h"
#include "Core/PlayerControllerPM.h"
#include "Developer/Windows/LiveCodingServer/Public/ILiveCodingServer.h"
#include "Logging/StructuredLog.h"
#include "Pawns/PawnPlayerPM.h"
#include "Core/PlayerStatePM.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Spawning/PlayerSpawnLocation.h"


AGameModePM::AGameModePM()
{
	GameStateClass = AGameStatePM::StaticClass();
	PlayerControllerClass = APlayerControllerPM::StaticClass();
	PlayerStateClass = APlayerStatePM::StaticClass();
	DefaultPawnClass = APawnPlayerPM::StaticClass();
}

void AGameModePM::BeginPlay()
{
	Super::BeginPlay();
	UE_LOGFMT(LogTemp, Warning, "GameModePM::BeginPlay()");
}

AActor* AGameModePM::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerSpawnLocation*> FoundSpawnLocations;

	if (SpawnPointClass)
	{
		for (TActorIterator<APlayerSpawnLocation> It(GetWorld()); It; ++It)
		{
			if (It->IsA(SpawnPointClass))
			{
				FoundSpawnLocations.Add(*It);
			}
		}
	}

	if (Player)
	{
		if (FoundSpawnLocations.Num() > 0)
		{
			return FoundSpawnLocations[0];
		}
	}
	
	return this;
}

void AGameModePM::InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer)
{
	// if (DefaultPawnClass && StartSpot && NewPlayer)
	// {
	// 	FActorSpawnParameters SpawnParameters;
	// 	SpawnParameters.Owner = NewPlayer;
	// 	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//
	// 	if (UWorld* World = GetWorld())
	// 	{
	// 		if (APawn* OwningPawn = NewPlayer->GetPawn())
	// 		{
	// 			NewPlayer->UnPossess();
	// 			OwningPawn->Destroy();
	// 		}
	//
	// 		if (APawn* NewPawn = World->SpawnActor<APawn>(DefaultPawnClass, StartSpot->GetActorLocation(), StartSpot->GetActorRotation(), SpawnParameters))
	// 		{
	// 			NewPlayer->Possess(NewPawn);
	// 		}
	// 	}
	// }
}
