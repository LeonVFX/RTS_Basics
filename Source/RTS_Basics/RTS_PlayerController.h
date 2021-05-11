// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTS_Unit.h"
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
	
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

public:
	
	// Array to hold all Selected Units
	UPROPERTY(VisibleAnywhere, Category = "Debug Var")
	TArray<AActor*> SelectedUnits;

	// Decal Values
	UPROPERTY(EditAnywhere, Category = "Decal")
	class UMaterialInstance* ActionDecal;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalSize = FVector(5.0, 64.0, 64.0);

	UPROPERTY(EditAnywhere, Category = "Decal")
	FRotator DecalRotation = FRotator(90.0, 0.0, 0.0);

	// Unit Values
	UPROPERTY(EditAnywhere, Category = "Spawnables")
	TSubclassOf<class ARTS_Unit> UnitToSpawn;

	// AI Values
	UPROPERTY(EditAnywhere, Category = "AI Values")
	float MovementTolerance = 90.0f;

private:
	UFUNCTION()
	void SelectUnit();

	UFUNCTION()
	void MoveUnits();

	UFUNCTION(Server, Reliable)
	void Server_MoveUnits(const TArray<int32>& Units, const FVector& TargetLocation, float Tolerance);
	void Server_MoveUnits_Implementation(const TArray<int32>& Units, const FVector& TargetLocation, float Tolerance);
	
	UFUNCTION()
	void MoveSingleUnit(AActor* Actor, const FVector& TargetLocation, float Tolerance);

	UFUNCTION()
	void SpawnUnit();

	UFUNCTION( Server, Reliable )
	void Server_SpawnUnit(APlayerController* PC, const FVector& _SpawnLocation);
	void Server_SpawnUnit_Implementation(APlayerController* PC, const FVector& _SpawnLocation);

	UFUNCTION()
	void AuthSpawnUnit(APlayerController* PC, const FVector& _SpawnLocation);
};