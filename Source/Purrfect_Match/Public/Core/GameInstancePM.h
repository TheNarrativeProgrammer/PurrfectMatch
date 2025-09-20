// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstancePM.generated.h"

/**
 * 
 */
UCLASS()
class PURRFECT_MATCH_API UGameInstancePM : public UGameInstance
{
	GENERATED_BODY()

public:
	//UGameInstancePM(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;
	
};
