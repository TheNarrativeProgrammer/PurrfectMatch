// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerControllerPM.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Logging/StructuredLog.h"

void APlayerControllerPM::SetupInputComponent()
{
	UE_LOGFMT(LogTemp, Warning, "SetupInputComponent Controller");
	Super::SetupInputComponent();

	if (IsValid(InputComponent))
	{
		TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputLocalPlayerSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		if (IsValid(EnhancedInputLocalPlayerSubsystem))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(PlayerInputMappingContext, 0);
		}

		if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			if (IsValid(IAMoveHorizontal))
			{
				EnhancedInputComponent->BindAction(IAMoveHorizontal, ETriggerEvent::Triggered, this, &APlayerControllerPM::MoveHorizontal);
			}

			if (IsValid(IAMoveVertical))
			{
				EnhancedInputComponent->BindAction(IAMoveVertical, ETriggerEvent::Triggered, this, &APlayerControllerPM::MoveVertical);
			}
		}
	}
}

void APlayerControllerPM::MoveHorizontal(const FInputActionValue& InputActionValue)
{
	const float AxisValue = InputActionValue.Get<float>();

	UE_LOGFMT(LogTemp, Warning, "side controller");
	if (TObjectPtr<APawn> PawnPlayer = GetPawn())
	{
		//PawnPlayer->AddMovementInput({1.0f, 0.0f, 0.0f}, AxisValue);
	}
}

void APlayerControllerPM::MoveVertical(const FInputActionValue& InputActionValue)
{
	const float AxisValue = InputActionValue.Get<float>();

	UE_LOGFMT(LogTemp, Warning, "Move vert");
	if (TObjectPtr<APawn> PawnPlayer = GetPawn())
	{
		//PawnPlayer->AddMovementInput({0.0f, 0.0f, 1.0f}, AxisValue);
	}
}
