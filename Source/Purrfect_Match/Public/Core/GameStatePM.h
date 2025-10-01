// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstancePM.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameBoardSendLevelPropertiesSignature, int32, totalAffectionNeeded, int32, level);

//PlayerPawn
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnPlayerRequestTileLocationSignature, int32, leftIndex);

//PlayerState
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerStateScoreChangeSignature, int32, oldScore, int32, newScore, int32, pointChange);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayerStateLiveChangeSignature, int32, oldLives, int32, newLives, int32, livesChange);

//GameState - Match states
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateRestartLevelSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateRestartGameSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateMatchEndSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateLevelCompletedSignature);

//GameMode
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameModeLoadLevelBoardSignature, ELevelStage, LevelStage);




UCLASS()
class PURRFECT_MATCH_API AGameStatePM : public AGameState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	//gameboard
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardPopulatedSiganature GameBoardPopulatedDelegate;

	UPROPERTY(Blueprintcallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardSendBoardDimensionsSignature GameBoardSendBoardDimensionsDelegate;

	UPROPERTY(Blueprintcallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardSwitchTilesSignature GameBoardSwitchTilesDelegate;

	UPROPERTY(Blueprintcallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FGameBoardSendLevelPropertiesSignature GameBoardSendLevelPropertiesDelegate;
	

	//pawn player
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintCallable, Category = "GameBoard Delegate")
	FGameBoardOnRequestSendTileLocationSiganature GameBoardOnRequestSendTileLocationDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "Gameboard Delegate")
	FPawnPlayerRequestTileLocationSignature PawnPlayerRequestTileLocationDelegate;

	//PlayerState
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "PlayerState Delegate")
	FPlayerStateScoreChangeSignature PlayerStateScoreChangeDelegate;


	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "PlayerState Delegate")
	FPlayerStateLiveChangeSignature PlayerStateLiveChangeDelegate;

	//Game State - match state
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "GameState Delegate")
	FGameStateRestartLevelSignature GameStateRestartLevelDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "GameState Delegate")
	FGameStateRestartGameSignature GameStateRestartGameDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "GameState Delegate")
	FGameStateMatchEndSignature GameStateMatchEndDelegate;

	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "GameState Delegate")
	FGameStateLevelCompletedSignature GameStateLevelCompletedDelegate;
	

	//GameMode
	UPROPERTY(BlueprintCallable, Blueprintable, BlueprintAssignable, Category = "GameState Delegate")
	FGameModeLoadLevelBoardSignature GameModeLoadLevelBoardDelegate;

	

	UFUNCTION(BlueprintCallable, Category = "Match State")
	void HandleRestartingLevel();

	UFUNCTION(BlueprintCallable, Category = "Match State")
	void HandleRestartingGame();

	virtual void HandleMatchHasEnded() override;
	
	


	
	
};
