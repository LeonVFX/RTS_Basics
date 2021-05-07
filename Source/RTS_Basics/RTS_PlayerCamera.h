// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTS_PlayerCamera.generated.h"

UCLASS()
class RTS_BASICS_API ARTS_PlayerCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTS_PlayerCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Custom Properties

	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY()
	class APlayerController* PC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan")
	float Margin = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pan")
	float PanSpeed = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Zoom")
	float MaxZoom = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Zoom")
	float MinZoom = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Zoom")
	float ZoomIncrement = 100;

	// Viewport Specifics
	UPROPERTY()
	bool bViewportSet = false;

	UPROPERTY()
	int32 ScreenSizeX;

	UPROPERTY()
	int32 ScreenSizeY;

private:
	// Custom Functions
	UFUNCTION()
	FVector GetCameraPanDirection() const;

	UFUNCTION()
	void MoveCamera(const FVector& PanDirection);

	UFUNCTION()
	void CameraZoomIn();

	UFUNCTION()
	void CameraZoomOut();

	UFUNCTION()
	void GetViewportDimensions();
};