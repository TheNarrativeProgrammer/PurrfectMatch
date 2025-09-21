// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerPM.generated.h"

struct FInputActionValue;
/**
 * 
 */
class UInputMappingContext;
class UInputAction;
UCLASS()
class PURRFECT_MATCH_API APlayerControllerPM : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> IAMoveHorizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input|Actions")
	TObjectPtr<UInputAction> IAMoveVertical;

	UFUNCTION()
	void MoveHorizontal(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void MoveVertical(const FInputActionValue& InputActionValue);
	
	
};
