// Fill out your copyright notice in the Description page of Project Settings.


#include "SKill1.h"

#include "BehaviorTree/BlackboardComponent.h"

USKill1::USKill1(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Skill 1");
}

EBTNodeResult::Type USKill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*
	AAIControllerBase* const AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	APawn* const ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* const ControllingMonster = Cast<AMonsterBase>(ControllingPawn);
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;

	
	//공격을 날릴 방향을 넘겨줌
	ControllingMonster->Attack();
	
	*/
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}