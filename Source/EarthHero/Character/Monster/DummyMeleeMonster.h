// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "DummyMeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyMeleeMonster : public AMonsterBase
{
	GENERATED_BODY()

	ADummyMeleeMonster();

	virtual void Attack() override;
};
