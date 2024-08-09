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

	virtual void Skill1() override;
	virtual void Skill2() override;
	virtual void Skill3() override;
	virtual void Skill4() override;
	
	FTimerHandle TimerHandle1, TimerHandle2, TimerHandle3, TimerHandle4;

	void SetSkill1CoolTime();
	void SetSkill2CoolTime();
	void SetSkill3CoolTime();
	void SetSkill4CoolTime();
	
	float Skill1CoolTime = 19.f;
	float Skill2CoolTime;
	float Skill3CoolTime;
	float Skill4CoolTime;
	
protected:
	virtual void BeginPlay() override;
};
