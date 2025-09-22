// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Binding/DelegateBindingCompBoardManager.h"
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
	}
}
