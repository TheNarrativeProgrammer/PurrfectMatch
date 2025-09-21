// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModePM.generated.h"

/**
 * 
 */
UCLASS()
class PURRFECT_MATCH_API AGameModePM : public AGameMode
{
	GENERATED_BODY()


	public:
	AGameModePM();
	
	virtual void BeginPlay() override;
	
};
