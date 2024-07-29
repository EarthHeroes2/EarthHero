// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/Character/EHCharacter.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API USkillBase : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	AEHCharacter* GetTargetPlayer(AAIControllerBase* AIController);
};
