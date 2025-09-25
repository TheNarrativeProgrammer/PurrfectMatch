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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameBoardSwitchTilesSignature, int32, indexLeft, int32, indexRight);

//PlayerPawn
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnPlayerRequestTileLocationSignature, int32, leftIndex);

UENUM(Blueprintable, BlueprintType)
enum ELevelStage : uint8
{
	MAIN_MENU,
	TUTORIAL_MENU,
	LEVEL1_STORY,
	LEVEL1_GAMEPLAY,
	LEVEL2_STORY,
	LEVEL2_GAMEPLAY,
	LEVEL3_STORY,
	LEVEL3_GAMEPLAY,
	ENDING_STORY,
	ENDING_CREDITS,
};

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

	UPROPERTY(Blueprintcallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardSwitchTilesSignature GameBoardSwitchTilesDelegate;

	//pawn player
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintCallable, Category = "GameBoard Delegate")
	FGameBoardOnRequestSendTileLocationSiganature GameBoardOnRequestSendTileLocationDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FPawnPlayerRequestTileLocationSignature PawnPlayerRequestTileLocationDelegate;

	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = "Level Stages")
	TEnumAsByte<ELevelStage> CurrentLevelStage;

	UFUNCTION(BlueprintCallable, Category = "Level Stages")
	void SetLevelStage(TEnumAsByte<ELevelStage> LevelStage);

	UFUNCTION(BlueprintPure, Category = "Level Stages")
	TEnumAsByte<ELevelStage> GetLevelStage() const {return CurrentLevelStage;}


	
	
};
