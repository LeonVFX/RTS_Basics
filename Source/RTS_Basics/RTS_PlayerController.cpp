// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_PlayerController.h"
#include "RTS_Unit.h"

#include "Kismet/GameplayStatics.h"

ARTS_PlayerController::ARTS_PlayerController()
{
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

// Called every frame
void ARTS_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARTS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Select", IE_Pressed, this, &ARTS_PlayerController::SelectUnit);
	InputComponent->BindAction("Action", IE_Pressed, this, &ARTS_PlayerController::MoveUnits);
}

void ARTS_PlayerController::SelectUnit()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, HitResult);

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());

	if (HitResult.Actor.IsValid())
	{
		if (HitResult.GetActor()->IsA(ARTS_Unit::StaticClass()))
		{
			AActor* AUnit = HitResult.GetActor();
			ARTS_Unit* Unit = static_cast<ARTS_Unit*>(AUnit);
			
			if (!Unit->bSelected)
				SelectedUnits.Add(AUnit);
			else
				SelectedUnits.Remove(AUnit);

			Unit->Select();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
		}
	}
}

void ARTS_PlayerController::MoveUnits()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, HitResult);

	// Play Decal at Mouse Position
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ActionDecal, DecalSize, HitResult.Location, DecalRotation, 0.2f);

	// Call the move function
	if (SelectedUnits.Num() != 0)
		for (AActor* AUnit : SelectedUnits)
		{
			float tolerance = (SelectedUnits.Num() - 1) * MovementTolerance;
			static_cast<ARTS_Unit*>(AUnit)->Move(HitResult.Location, tolerance);
		}
}