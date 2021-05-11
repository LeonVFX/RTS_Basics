// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "RTS_UnitInterface.h"
#include "RTS_Unit.generated.h"

UCLASS()
class RTS_BASICS_API ARTS_Unit : public ACharacter, public IRTS_UnitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARTS_Unit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	UPROPERTY(VisibleAnywhere, Replicated, Category = "UnitDetails")
	int32 ID;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	class APlayerController* OwningPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class AAIController* AIC;

	// Decal
	UPROPERTY(EditAnywhere, Category = "Decal")
	class UDecalComponent* SelectionDecal;

	UPROPERTY(EditAnywhere)
	bool bSelected = false;

	// Interface Functions
	UFUNCTION()
	virtual void Select() override;

	UFUNCTION( Client, Reliable )
	void Client_Select();
	void Client_Select_Implementation();
};