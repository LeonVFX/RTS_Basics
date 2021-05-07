// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_Unit.h"

#include "AIController.h"
#include "Components/DecalComponent.h"
#include "Net/UnrealNetwork.h"

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

	if(!IsLocallyControlled())
		return;

	AIC = Cast<AAIController>(GetController());
	if(!ensure(AIC != nullptr)) return;

	//UE_LOG(LogTemp, Warning, TEXT("Begin Play Owner: %s"), *GetOwner()->);
	
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

void ARTS_Unit::Select()
{
	Client_Select();
}

void ARTS_Unit::Client_Select_Implementation()
{
	if (!bSelected)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Unit Selected!"));
		SelectionDecal->SetVisibility(true);
		bSelected = true;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Unit Deselected!"));
		SelectionDecal->SetVisibility(false);
		bSelected = false;
	}
}

void ARTS_Unit::Move(const FVector& targetLocation, float tolerance) const
{
	UE_LOG(LogTemp, Warning, TEXT("Moving - Step 1"));
	UE_LOG(LogTemp, Warning, TEXT("Role: %i"), GetLocalRole());
	
	if (HasAuthority())
		AIC->MoveToLocation(targetLocation, tolerance);
	else
		Server_Move(targetLocation, tolerance);
}

void ARTS_Unit::Server_Move_Implementation(const FVector& targetLocation, float tolerance) const
{
	UE_LOG(LogTemp, Warning, TEXT("Moving - Step 2"));
	AuthMove(targetLocation, tolerance);
}

void ARTS_Unit::AuthMove(const FVector& targetLocation, float tolerance) const
{
	UE_LOG(LogTemp, Warning, TEXT("Moving - Step 3"));
	if (!HasAuthority())
		return;
	
	AIC->MoveToLocation(targetLocation, tolerance);
}

// void ARTS_Unit::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
// {
// 	DOREPLIFETIME( ARTS_Unit, OwningPlayer );
// }