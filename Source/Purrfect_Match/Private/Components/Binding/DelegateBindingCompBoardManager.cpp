// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Binding/DelegateBindingCompBoardManager.h"

#include "Components/TilePopulatorComponent.h"
#include "GameBoard/GameBoard.h"
#include "Managers/BoardManager.h"

void UDelegateBindingCompBoardManager::BindDelegates()
{
	Super::BindDelegates();

	if (GameStatePM)
	{
		if (ABoardManager* BoardManagerOwner = Cast<ABoardManager>(GetOwner()))
		{
			GameStatePM->GameBoardPopulatedDelegate.AddDynamic(BoardManagerOwner, &ABoardManager::PopulateBoardWithInitialTiles);
		}
		if (AGameBoard* GameBoardOwner = Cast<AGameBoard>(GetOwner()))
		{
			if (UTilePopulatorComponent* TilePopulatorComponent = GameBoardOwner->GetComponentByClass<UTilePopulatorComponent>())
			{
				GameStatePM->GameBoardPopulatedDelegate.AddDynamic(TilePopulatorComponent, &UTilePopulatorComponent::PopulateBoardWithInitialTiles);
			}
			
		}
	}
}
