// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Binding/DelegateBindingCompGameBoard.h"
#include "GameBoard/GameBoard.h"

void UDelegateBindingCompGameBoard::BindDelegates()
{
	Super::BindDelegates();

	if (GameStatePM)
	{
		if (AGameBoard* GameBoardOwner = Cast<AGameBoard>(GetOwner()))
		{
			GameStatePM->PawnPlayerRequestTileLocationDelegate.AddUniqueDynamic(GameBoardOwner, &AGameBoard::GetTileLocation);
			GameStatePM->GameBoardSwitchTilesDelegate.AddUniqueDynamic(GameBoardOwner, &AGameBoard::SwitchTiles);
			GameStatePM->GameStateRestartLevelDelegate.AddUniqueDynamic(GameBoardOwner, &AGameBoard::OnLevelRestarted);
			//GameStatePM->GameStateRestartGameDelegate.AddUniqueDynamic(GameBoardOwner, &AGameBoard::OnLevelRestarted);
			GameStatePM->GameModeLoadLevelBoardDelegate.AddUniqueDynamic(GameBoardOwner, &AGameBoard::OnLevelStarSetLevelStageAndtInitialze);
			GameStatePM->GameStateLevelCompletedDelegate.AddUniqueDynamic(GameBoardOwner, &AGameBoard::OnLevelCompletedStopBoard);
		}
	}
}
