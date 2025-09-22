// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStatePM.generated.h"

/**
 * 
 */

//GameBoard
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameBoardPopulatedSiganature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameBoardOnRequestSendTileLocationSiganature, FVector, location);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameBoardSendBoardDimensionsSignature, int32, width, int32, height);

//PlayerPawn
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnPlayerRequestTileLocationSignature, int32, leftIndex);

UCLASS()
class PURRFECT_MATCH_API AGameStatePM : public AGameState
{
	GENERATED_BODY()
public:
	//gameboard
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardPopulatedSiganature GameBoardPopulatedDelegate;

	UPROPERTY(Blueprintcallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardSendBoardDimensionsSignature GameBoardSendBoardDimensionsDelegate;

	//pawn player
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintCallable, Category = "GameBoard Delegate")
	FGameBoardOnRequestSendTileLocationSiganature GameBoardOnRequestSendTileLocationDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FPawnPlayerRequestTileLocationSignature PawnPlayerRequestTileLocationDelegate;
	
};
