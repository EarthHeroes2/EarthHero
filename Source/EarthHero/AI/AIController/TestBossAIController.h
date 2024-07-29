// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "TestBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ATestBossAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
	ATestBossAIController(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;


	
	void Cleanser();


	FTimerHandle TimerHandle1, TimerHandle2;
};
