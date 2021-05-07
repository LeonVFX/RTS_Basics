// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_Unit.h"

#include "AIController.h"
#include "Components/DecalComponent.h"

// Sets default values
ARTS_Unit::ARTS_Unit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Selection Decal"));
	if (!ensure(SelectionDecal != nullptr)) return;

	SelectionDecal->SetupAttachment(RootComponent);
	SelectionDecal->SetRelativeLocation(FVector(-10.0, 0.0, -100.0));
	SelectionDecal->SetRelativeRotation(FRotator(90.0, 0.0, 0.0));
	SelectionDecal->DecalSize.X = 5.0;
	SelectionDecal->DecalSize.Y = 64.0;
	SelectionDecal->DecalSize.Z = 64.0;
	SelectionDecal->SetVisibility(false);
}

// Called when the game starts or when spawned
void ARTS_Unit::BeginPlay()
{
	Super::BeginPlay();

	AIC = Cast<AAIController>(GetController());

	if (!HasAuthority())
		return;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called every frame
void ARTS_Unit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTS_Unit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARTS_Unit::Select_Implementation()
{
	if (!bSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit Selected!"));
		SelectionDecal->SetVisibility(true);
		bSelected = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unit Deselected!"));
		SelectionDecal->SetVisibility(false);
		bSelected = false;
	}
}

void ARTS_Unit::Move_Implementation(const FVector& targetLocation, float tolerance)
{
	AIC->MoveToLocation(targetLocation, tolerance);
}