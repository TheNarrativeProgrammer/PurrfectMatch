// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameInstancePM.h"

void UGameInstancePM::Init()
{
	Super::Init();
	SetLevelStage(ELevelStage::MAIN_MENU);
}

void UGameInstancePM::SetLevelStage(TEnumAsByte<ELevelStage> LevelStage)
{
	CurrentLevelStage = LevelStage;
}
