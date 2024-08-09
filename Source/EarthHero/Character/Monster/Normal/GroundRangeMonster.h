// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/GroundMonster.h"
#include "GroundRangeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AGroundRangeMonster : public AGroundMonster
{
	GENERATED_BODY()

	AGroundRangeMonster();

	virtual void Attack(FVector SpawnNormalVector) override;
};
