// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "RTS_GameInstance.h"
#include "AIController.h"
#include "RTS_Unit.h"

ARTS_PlayerController::ARTS_PlayerController()
{
	bReplicates = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ARTS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ARTS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Select", IE_Pressed, this, &ARTS_PlayerController::SelectUnit);
	InputComponent->BindAction("Action", IE_Pressed, this, &ARTS_PlayerController::MoveUnits);
	InputComponent->BindAction("SpawnUnit", IE_Pressed, this, &ARTS_PlayerController::SpawnUnit);
}

void ARTS_PlayerController::SelectUnit()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, HitResult);

	if (HitResult.Actor.IsValid())
	{
		if (HitResult.GetActor()->IsA(ARTS_Unit::StaticClass()))
		{
			AActor* AUnit = HitResult.GetActor();
			ARTS_Unit* Unit = static_cast<ARTS_Unit*>(AUnit);

			// If Owner Only
			if (Unit->OwningPlayer != this)
				return;
			
			if (!Unit->bSelected)
				SelectedUnits.Add(AUnit);
			else
				SelectedUnits.Remove(AUnit);

			Unit->Select();
		}
	}
}

void ARTS_PlayerController::MoveUnits()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, HitResult);

	// Play Decal at Mouse Position
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ActionDecal, DecalSize, HitResult.Location, DecalRotation, 0.2f);

	TArray<int32> Units;
	if (SelectedUnits.Num() != 0)
		for (AActor* AUnit : SelectedUnits)
		{
			ARTS_Unit* _RTS_Unit = static_cast<ARTS_Unit*>(AUnit);
			Units.Add(_RTS_Unit->ID);
		}
	
	if (HasAuthority())
	{
		for (int32 Key : Units)
			MoveSingleUnit(GetGameInstance<URTS_GameInstance>()->UnitMap[Key], HitResult.Location, (SelectedUnits.Num() - 1) * MovementTolerance);
	}
	else
		Server_MoveUnits(Units, HitResult.Location, (SelectedUnits.Num() - 1) * MovementTolerance);
}

void ARTS_PlayerController::Server_MoveUnits_Implementation(const TArray<int32>& Units, const FVector& TargetLocation, float Tolerance)
{
	for (int32 Key : Units)
		MoveSingleUnit(GetGameInstance<URTS_GameInstance>()->UnitMap[Key], TargetLocation, Tolerance);
}

void ARTS_PlayerController::MoveSingleUnit(AActor* Actor, const FVector& TargetLocation, float Tolerance)
{
	static_cast<ARTS_Unit*>(Actor)->AIC->MoveToLocation(TargetLocation, Tolerance);
}

void ARTS_PlayerController::SpawnUnit()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, HitResult);
	
	if (HasAuthority())
		AuthSpawnUnit(this, HitResult.Location);
	else
		Server_SpawnUnit(this, HitResult.Location);
}

void ARTS_PlayerController::Server_SpawnUnit_Implementation(APlayerController* PC, const FVector& _SpawnLocation)
{
	AuthSpawnUnit(PC, _SpawnLocation);
}

void ARTS_PlayerController::AuthSpawnUnit(APlayerController* PC, const FVector& _SpawnLocation)
{
	if (!HasAuthority())
		return;
	
	if (!UnitToSpawn)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters SpawnParams;
	// Apparently this is doing nothing...
	SpawnParams.Owner = PC;

	ARTS_Unit* NewUnit = World->SpawnActor<ARTS_Unit>(UnitToSpawn, _SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	NewUnit->OwningPlayer = PC;
}