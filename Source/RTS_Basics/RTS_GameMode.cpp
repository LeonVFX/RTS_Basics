// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_GameMode.h"
#include "RTS_PlayerCamera.h"
#include "RTS_PlayerController.h"
#include "UObject/ConstructorHelpers.h"

ARTS_GameMode::ARTS_GameMode()
{
	// set player controller to our Blueprinted player
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/RTS_Basics/Blueprints/BP_RTS_PlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
		PlayerControllerClass = PlayerControllerBPClass.Class;
	
	// set default pawn class to our Blueprinted player
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/RTS_Basics/Blueprints/BP_RTS_PlayerCamera"));
	if (PlayerPawnBPClass.Class != nullptr)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	//DefaultPawnClass = ARTS_PlayerCamera::StaticClass();
}