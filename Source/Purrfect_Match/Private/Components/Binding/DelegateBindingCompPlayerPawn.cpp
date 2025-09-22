// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Binding/DelegateBindingCompPlayerPawn.h"
#include "Pawns/PawnPlayerPM.h"

void UDelegateBindingCompPlayerPawn::BindDelegates()
{
	Super::BindDelegates();

	if (GameStatePM)
	{
		if (APawnPlayerPM* PawnPlayerPMOwner = Cast<APawnPlayerPM>(GetOwner()))
		{
			GameStatePM->GameBoardPopulatedDelegate.AddUniqueDynamic(PawnPlayerPMOwner, &APawnPlayerPM::SetStartLocation);
			GameStatePM->GameBoardOnRequestSendTileLocationDelegate.AddUniqueDynamic(PawnPlayerPMOwner,&APawnPlayerPM::SetPawnLocation);
			GameStatePM->GameBoardSendBoardDimensionsDelegate.AddUniqueDynamic(PawnPlayerPMOwner, &APawnPlayerPM::SetBoardWithAndHeight);
		}
	}
}
