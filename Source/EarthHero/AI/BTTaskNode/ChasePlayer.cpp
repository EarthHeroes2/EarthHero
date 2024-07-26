// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("11111111111111111111111111111111111"));
	
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}