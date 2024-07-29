// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/GroundMonster.h"
#include "MidBoss1Monster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMidBoss1Monster : public AGroundMonster
{
	GENERATED_BODY()

	AMidBoss1Monster();
	virtual void Attack() override;
};
