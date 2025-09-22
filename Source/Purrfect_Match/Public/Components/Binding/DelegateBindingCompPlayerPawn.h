// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Binding/DelegateBindingComponent.h"
#include "DelegateBindingCompPlayerPawn.generated.h"

/**
 * 
 */
UCLASS()
class PURRFECT_MATCH_API UDelegateBindingCompPlayerPawn : public UDelegateBindingComponent
{
	GENERATED_BODY()

	virtual void BindDelegates() override;
	
};
