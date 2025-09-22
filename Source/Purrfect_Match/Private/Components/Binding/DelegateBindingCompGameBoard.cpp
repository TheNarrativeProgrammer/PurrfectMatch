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
		}
	}
}
