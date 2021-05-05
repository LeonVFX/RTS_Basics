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
	// Constructor
	ARTS_PlayerController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

public:
	// Array to hold all Selected Units
	TArray<AActor*> SelectedUnits;

	UPROPERTY(EditAnywhere, Category = "Decal")
	class UMaterialInstance* ActionDecal;

private:
	UFUNCTION()
	void SelectUnit();

	UFUNCTION()
	void MoveUnits();
};