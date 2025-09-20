// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstancePurr.generated.h"

/**
 * 
 */
UCLASS()
class PURRFECT_MATCH_API UGameInstancePurr : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
};
