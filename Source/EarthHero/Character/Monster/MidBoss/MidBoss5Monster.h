// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/GroundMonster.h"
#include "MidBoss5Monster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMidBoss5Monster : public AGroundMonster
{
	GENERATED_BODY()

	AMidBoss5Monster();
	virtual void Attack() override;
	virtual void Skill1() override;
	virtual void Skill2() override;
};
