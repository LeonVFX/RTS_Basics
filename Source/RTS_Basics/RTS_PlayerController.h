// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_BASICS_API ARTS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Array to hold all Selected Units
	TArray<AActor*> SelectedUnits;
};