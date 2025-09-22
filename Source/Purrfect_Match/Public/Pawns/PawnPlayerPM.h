// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnPlayerPM.generated.h"

struct FInputActionValue;
class UFloatingPawnMovement;
class UStaticMeshComponent;
class AGameBoard;
class UDelegateBindingCompPlayerPawn;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PURRFECT_MATCH_API APawnPlayerPM : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnPlayerPM();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DelegateBindingComponent")
	TObjectPtr<UDelegateBindingCompPlayerPawn> DelegateBindingCompPlayerPawn;

	UPROPERTY(EditAnywhere, Category="Gameboard")
	TObjectPtr<AGameBoard> GameBoard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameboard")
	int32 indexLeft = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameboard")
	int32 indexRight = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameboard")
	int32 boardWidth = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameboard")
	int32 boardHeight = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameboard")
	int32 lastIndex = 71;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameboard")
	int32 firstIndexOfLastRow = 66;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> IAMoveHorizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> IAMoveVertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> IASwitchTiles;

	UFUNCTION()
	void MoveHorizontal(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void MoveVertical(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void SwitchTiles(const FInputActionValue& InputActionValue);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* SceneComponentRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UFloatingPawnMovement* FloatingPawnMovement;

	UFUNCTION()
	void SetStartLocation();

	UFUNCTION()
	void SetPawnLocation(FVector Location);

	UFUNCTION()
	void SetBoardWithAndHeight(int32 InBoardWidth, int32 InHeight);

	

};
