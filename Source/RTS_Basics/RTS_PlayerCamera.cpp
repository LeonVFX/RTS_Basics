// Fill out your copyright notice in the Description page of Project Settings.
#include "RTS_PlayerCamera.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Templates/Function.h"
#include "Async/Future.h"

// Sets default values
ARTS_PlayerCamera::ARTS_PlayerCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(Root);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-70.0, 0, 0));
	SpringArm->TargetArmLength = 1500;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
}

// Called when the game starts or when spawned
void ARTS_PlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
	PC = Cast<APlayerController>(GetController());

	/// <summary>
	/// ASYNC FUNTION: Gets the Viewport Dimentions once they are properly initialized
	/// </summary>
	TUniqueFunction<void()> ViewportThread = [&]()
	{
		GetViewportDimensions();
	};
	Async(EAsyncExecution::Thread, MoveTemp(ViewportThread));

	UE_LOG(LogTemp, Warning, TEXT("%i, %i"), ScreenSizeX, ScreenSizeY);
}

// Called every frame
void ARTS_PlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera(GetCameraPanDirection());
}

// Called to bind functionality to input
void ARTS_PlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ARTS_PlayerCamera::CameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ARTS_PlayerCamera::CameraZoomOut);
}

FVector ARTS_PlayerCamera::GetCameraPanDirection()
{
	float MousePosX;
	float MousePosY;
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

	return FVector(CamDirectionX, CamDirectionY, 0);
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

void ARTS_PlayerCamera::GetViewportDimensions()
{
	while (ScreenSizeX == 0 && ScreenSizeY == 0)
		PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
}