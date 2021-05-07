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
	InputComponent->BindAction("SpawnUnit", IE_Pressed, this, &ARTS_PlayerController::SpawnUnit);
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
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
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
			ARTS_Unit* _RTS_Unit = static_cast<ARTS_Unit*>(AUnit);
			const float tolerance = (SelectedUnits.Num() - 1) * MovementTolerance;
			_RTS_Unit->Move(HitResult.Location, tolerance);
		}
}

void ARTS_PlayerController::SpawnUnit()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, HitResult);
	
	if (HasAuthority())
	{
		AuthSpawnUnit(this, HitResult.Location);
		//UE_LOG(LogTemp, Warning, TEXT("Has authority"));
	}
	else
	{
		Server_SpawnUnit(this, HitResult.Location);
		//UE_LOG(LogTemp, Warning, TEXT("Does not have authority"));
	}
}

void ARTS_PlayerController::Server_SpawnUnit_Implementation(APlayerController* PC, const FVector& _SpawnLocation)
{
	AuthSpawnUnit(this, _SpawnLocation);
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
	UE_LOG(LogTemp, Warning, TEXT("Server: %s"), *GetName());

	if (this != PC)
		NewUnit->SetOwner(PC);
	//Client_Ownership(NewUnit);
}

// void ARTS_PlayerController::Client_Ownership_Implementation(ARTS_Unit* unit)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Client: %s"), *GetName());
// 	
// 	while (unit == nullptr)
// 		continue;
//
// 	unit->SetOwner(this);
// }