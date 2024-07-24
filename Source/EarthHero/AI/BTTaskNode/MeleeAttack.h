// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UMeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UMeleeAttack(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
