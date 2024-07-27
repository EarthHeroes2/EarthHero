// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "DummyRangeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyRangeMonster : public AMonsterBase
{
	GENERATED_BODY()

	ADummyRangeMonster();
	virtual void Attack(FVector SpawnNormalVector) override;

	TSubclassOf<AActor> TestBulletClass;
};
