// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class EARTHHERO_API UFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

	UFindPatrolPos(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	float SearchRadius = 1000.f;
};
