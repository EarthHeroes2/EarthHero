// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Skill1.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API USkill1 : public USkillBase
{
	GENERATED_BODY()

	USkill1(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
