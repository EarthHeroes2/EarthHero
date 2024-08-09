// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/AirMonster.h"
#include "AirMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AAirMeleeMonster : public AAirMonster
{
	GENERATED_BODY()

	AAirMeleeMonster();

	virtual void Attack() override;
};
