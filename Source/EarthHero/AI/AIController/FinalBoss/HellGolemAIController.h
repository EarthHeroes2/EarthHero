// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "HellGolemAIController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AHellGolemAIController : public AAIControllerBase
{
	GENERATED_BODY()

	AHellGolemAIController(FObjectInitializer const& ObjectInitializer);

	void Skill1();
	void Skill2();


	FTimerHandle TimerHandle1, TimerHandle2;
};
