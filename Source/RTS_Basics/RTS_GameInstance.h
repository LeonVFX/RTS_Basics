// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(Exec)
	void Host() const;

	UFUNCTION(Exec)
	void Join(const FString& Address) const;
};
