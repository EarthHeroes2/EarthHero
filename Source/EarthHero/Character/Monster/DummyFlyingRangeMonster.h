// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "DummyFlyingRangeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyFlyingRangeMonster : public AMonsterBase
{
	GENERATED_BODY()

	ADummyFlyingRangeMonster();

	virtual void Attack(FVector SpawnNormalVector) override;

	TSubclassOf<AActor> TestBulletClass;
};
