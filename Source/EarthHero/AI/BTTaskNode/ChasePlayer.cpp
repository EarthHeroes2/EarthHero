// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/TestAIController.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("ChasePlayer");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* AIController = Cast<ATestAIController>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;
		
	return EBTNodeResult::Succeeded;
}