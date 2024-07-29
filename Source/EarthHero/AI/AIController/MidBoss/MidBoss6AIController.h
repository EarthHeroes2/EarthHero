// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "MidBoss6AIController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMidBoss6AIController : public AAIControllerBase
{
	GENERATED_BODY()

	AMidBoss6AIController(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;


	
	void Skill1();
	void Skill2();


	FTimerHandle TimerHandle1, TimerHandle2;
};
