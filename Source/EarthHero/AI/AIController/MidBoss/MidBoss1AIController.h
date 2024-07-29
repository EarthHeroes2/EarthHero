// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "MidBoss1AIController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMidBoss1AIController : public AAIControllerBase
{
	GENERATED_BODY()
	
	AMidBoss1AIController(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;


	
	void Cleanser();


	FTimerHandle TimerHandle1, TimerHandle2;
};
