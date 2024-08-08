// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/GroundMonster.h"
#include "GroundMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AGroundMeleeMonster : public AGroundMonster
{
	GENERATED_BODY()

	AGroundMeleeMonster();

	virtual void Attack() override;
};
