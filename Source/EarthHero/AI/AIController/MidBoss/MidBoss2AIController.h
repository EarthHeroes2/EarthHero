// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "MidBoss2AIController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMidBoss2AIController : public AAIControllerBase
{
	GENERATED_BODY()

	AMidBoss2AIController(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;


	
	void Skill1();
	void Skill2();


	FTimerHandle TimerHandle1, TimerHandle2;
};
