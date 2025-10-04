// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDPurrfectMatch.h"

#include "EnhancedInputComponent.h"
#include "Engine/InputDelegateBinding.h"


AHUDPurrfectMatch::AHUDPurrfectMatch()
{
	
}

void AHUDPurrfectMatch::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		UInputDelegateBinding::BindInputDelegates(GetClass(), PlayerController->InputComponent, this);
		
	}
}
