// Fill out your copyright notice in the Description page of Project Settings.
#include "RTS_PlayerCamera.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Templates/Function.h"

// Sets default values
ARTS_PlayerCamera::ARTS_PlayerCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	if (!ensure(Root != nullptr)) return;
	RootComponent = Root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	if (!ensure(SpringArm != nullptr)) return;
	SpringArm->SetupAttachment(Root);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-70.0, 0, 0));
	SpringArm->TargetArmLength = 1500;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (!ensure(Camera != nullptr)) return;
	Camera->SetupAttachment(SpringArm);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
	if (!ensure(FloatingPawnMovement != nullptr)) return;
}

// Called when the game starts or when spawned
void ARTS_PlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
	PC = Cast<APlayerController>(GetController());
	if (!ensure(PC != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PC->Player->GetName());

	GetViewportDimensions();
}

// Called every frame
void ARTS_PlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Safe net in case screen is not on focus
	if (ScreenSizeX != 0 && ScreenSizeY != 0)
		MoveCamera(GetCameraPanDirection());
}

// Called to bind functionality to input
void ARTS_PlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ARTS_PlayerCamera::CameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ARTS_PlayerCamera::CameraZoomOut);
}

FVector ARTS_PlayerCamera::GetCameraPanDirection() const
{
	float MousePosX = 0;
	float MousePosY = 0;
	float CamDirectionX = 0;
	float CamDirectionY = 0;

	PC->GetMousePosition(MousePosX, MousePosY);
	
	if (MousePosX <= Margin)
		CamDirectionY = -1;
	if (MousePosX >= ScreenSizeX - Margin)
		CamDirectionY = 1;
	if (MousePosY <= Margin)
		CamDirectionX = 1;
	if (MousePosY >= ScreenSizeY - Margin)
		CamDirectionX = -1;

	return FVector(CamDirectionX, CamDirectionY, 0.0f);
}

void ARTS_PlayerCamera::MoveCamera(const FVector& PanDirection)
{
	if (PanDirection == FVector::ZeroVector)
		return;
	else
		AddActorWorldOffset(PanDirection * PanSpeed);
}

void ARTS_PlayerCamera::CameraZoomIn()
{
	SpringArm->TargetArmLength = (SpringArm->TargetArmLength > MinZoom) ? SpringArm->TargetArmLength - ZoomIncrement : MinZoom;
}

void ARTS_PlayerCamera::CameraZoomOut()
{
	SpringArm->TargetArmLength = (SpringArm->TargetArmLength < MaxZoom) ? SpringArm->TargetArmLength + ZoomIncrement : MaxZoom;
}

/// <summary>
/// ASYNC FUNCTION: Gets the Viewport Dimensions once they are properly initialized
/// </summary>
void ARTS_PlayerCamera::GetViewportDimensions()
{
	TUniqueFunction<void()> ViewportThread = [&]()
	{
		while (ScreenSizeX == 0 && ScreenSizeY == 0)
			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
	};
	Async(EAsyncExecution::TaskGraph, MoveTemp(ViewportThread));
}