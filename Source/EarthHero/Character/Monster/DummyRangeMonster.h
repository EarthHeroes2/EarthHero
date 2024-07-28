// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GroundMonster.h"
#include "DummyRangeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyRangeMonster : public AGroundMonster
{
	GENERATED_BODY()

	ADummyRangeMonster();
	virtual void Attack(FVector SpawnNormalVector) override;

	TSubclassOf<AActor> TestBulletClass;
};
