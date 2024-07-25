// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API URangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	URangedAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
