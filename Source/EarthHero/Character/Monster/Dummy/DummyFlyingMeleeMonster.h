// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/AirMonster.h"
#include "DummyFlyingMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyFlyingMeleeMonster : public AAirMonster
{
	GENERATED_BODY()

	ADummyFlyingMeleeMonster();
	virtual void Attack() override;
};
