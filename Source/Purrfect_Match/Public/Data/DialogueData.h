// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PURRFECT_MATCH_API UDialogueData : public UDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueData")
	FText DialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueData")
	UTexture2D* SpeakerImage;
	
};
