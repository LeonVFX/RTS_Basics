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

	// Decal Values
	UPROPERTY(EditAnywhere, Category = "Decal")
	class UMaterialInstance* ActionDecal;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalSize = FVector(5.0, 64.0, 64.0);

	UPROPERTY(EditAnywhere, Category = "Decal")
	FRotator DecalRotation = FRotator(90.0, 0.0, 0.0);

	// AI Values
	UPROPERTY(EditAnywhere, Category = "AI Values")
	float MovementTolerance = 90.0f;

private:
	UFUNCTION()
	void SelectUnit();

	UFUNCTION()
	void MoveUnits();
};