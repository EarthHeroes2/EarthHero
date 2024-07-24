// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CheckPlayerInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UCheckPlayerInAttackRange : public UBTTaskNode
{
	GENERATED_BODY()

	UCheckPlayerInAttackRange(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
