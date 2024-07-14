// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameOverPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AGameOverPlayerState : public APlayerState
{
	GENERATED_BODY()
	FTimerHandle SetStatComponentTimerHandle;
	virtual void BeginPlay() override;

public:
	int TestVar = 1;
	void TestFunc();
};
