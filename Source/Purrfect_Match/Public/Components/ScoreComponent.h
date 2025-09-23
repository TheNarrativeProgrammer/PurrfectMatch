// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PURRFECT_MATCH_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScoreComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 score = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	int32 minimumMatchingForPoint = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Points")
	int32 pointsPerMatch = 5;

	UFUNCTION(BlueprintCallable)
	void UpdateScore(int32 pointsScored);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
