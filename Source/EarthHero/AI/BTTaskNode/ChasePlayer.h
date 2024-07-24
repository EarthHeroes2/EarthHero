// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()

	UChasePlayer(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
