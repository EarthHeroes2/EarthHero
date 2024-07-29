// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SKill1.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API USKill1 : public UBTTaskNode
{
	GENERATED_BODY()

	USKill1(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
