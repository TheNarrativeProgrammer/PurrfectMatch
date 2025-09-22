// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "GameplayTags.h"
#include "BoardManager.generated.h"

class AGameBoard;
class UDelegateBindingCompBoardManager;

UCLASS()
class PURRFECT_MATCH_API ABoardManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardManager();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding Component")
	TObjectPtr<UDelegateBindingCompBoardManager> DelegateBindingCompBoardManager;

	UPROPERTY(EditAnywhere)
	int32 level;

	UPROPERTY()
	FGameplayTag TagAffectionSun;

	UPROPERTY()
	FGameplayTag TagAffectionPlant;

	UPROPERTY()
	FGameplayTag TagAffectionFood;

	UPROPERTY()
	FGameplayTag TagAffectionToy;

	UPROPERTY()
	FGameplayTag TagGoalBath;

	UPROPERTY()
	FGameplayTag TagGoalPet;

	UPROPERTY()
	FGameplayTag TagGoalDog;

	UPROPERTY()
	float chanceOfGoalTile = 0.2f;

	UPROPERTY()
	int32 numberOfRowsAtLevelStart = 3;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Board Reference")
	TObjectPtr<AGameBoard> GameBoard = nullptr;

	UFUNCTION(BlueprintCallable)
	TArray<FGameplayTag> GenerateTileLine();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTileTag(int32 tileIconNumber);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PopulateBoardWithInitialTiles();

};
