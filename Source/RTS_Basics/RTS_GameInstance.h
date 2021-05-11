// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTS_GameMode.h"
#include "Engine/GameInstance.h"
#include "RTS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RTS_BASICS_API URTS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Units");
	int32 NextUnitID = 0;

	UPROPERTY(VisibleAnywhere, Category = "Units");
	TMap<int32, AActor*> UnitMap;

	UFUNCTION()
	int RegisterUnit(AActor* Unit);
	
	UFUNCTION(Exec)
	void Host() const;

	UFUNCTION(Exec)
	void Join(const FString& Address) const;
};
