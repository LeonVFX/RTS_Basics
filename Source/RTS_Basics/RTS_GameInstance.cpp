// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_GameInstance.h"

int URTS_GameInstance::RegisterUnit(AActor* Unit)
{
	UnitMap.Add(NextUnitID, Unit);
	return NextUnitID++;
}

void URTS_GameInstance::Host() const
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting..."));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/TopDownCPP/Maps/TopDownExampleMap?listen");
}

void URTS_GameInstance::Join(const FString& Address) const
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}