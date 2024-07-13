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


public:
	int TestVar;
	void TestFunc();
};
