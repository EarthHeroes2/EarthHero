// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "MeleeAIController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMeleeAIController : public AAIControllerBase
{
	GENERATED_BODY()
	
	EMonsterAttackType AttackType = Melee;
	
};
