// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindSafeWay.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UFindSafeWay : public UBTTaskNode
{
	GENERATED_BODY()

	UFindSafeWay(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
