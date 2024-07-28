// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "DummyFlyingMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyFlyingMeleeMonster : public AMonsterBase
{
	GENERATED_BODY()

	ADummyFlyingMeleeMonster();
	virtual void Attack(FVector SpawnNormalVector) override;
};
