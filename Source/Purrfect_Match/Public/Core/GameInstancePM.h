// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstancePM.generated.h"

/**
 * 
 */

UENUM(Blueprintable, BlueprintType)
enum ELevelStage : uint8
{
	MAIN_MENU,
	TUTORIAL1_MENU,
	TUTORIAL2_MENU,
	TUTORIAL3_MENU,
	LEVEL1_STORY,
	LEVEL1_GAMEPLAY,
	LEVEL2_STORY,
	LEVEL2_GAMEPLAY,
	LEVEL3_STORY,
	LEVEL3_GAMEPLAY,
	ENDING_STORY,
	ENDING_CREDITS,
	GAMEOVER_MENU,
};

class UMusicManagerComponent;
UCLASS()
class PURRFECT_MATCH_API UGameInstancePM : public UGameInstance
{
	GENERATED_BODY()

public:
	//UGameInstancePM(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;

protected:
	UPROPERTY(BlueprintReadWrite, Blueprintable, Category = "Level Stages")
	TEnumAsByte<ELevelStage> CurrentLevelStage;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Level Stages")
	void SetLevelStage(TEnumAsByte<ELevelStage> LevelStage);

	UFUNCTION(BlueprintPure, Category = "Level Stages")
	TEnumAsByte<ELevelStage> GetLevelStage() const { return CurrentLevelStage; }
	
};
