// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KeepSafeFlyDistance.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UKeepSafeFlyDistance : public UBTTaskNode
{
	GENERATED_BODY()

	UKeepSafeFlyDistance(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
