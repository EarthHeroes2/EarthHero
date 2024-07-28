// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GroundMonster.h"
#include "TestBossGroundMonster.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ATestBossGroundMonster : public AGroundMonster
{
	GENERATED_BODY()

	ATestBossGroundMonster();
	virtual void Attack() override;
};
