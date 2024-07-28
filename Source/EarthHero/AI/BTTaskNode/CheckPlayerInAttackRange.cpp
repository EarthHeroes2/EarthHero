// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPlayerInAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

UCheckPlayerInAttackRange::UCheckPlayerInAttackRange(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Check Player In Attack Range");
}

EBTNodeResult::Type UCheckPlayerInAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* const AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	APawn* const ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* const ControllingMonsterPawn = Cast<AMonsterBase>(ControllingPawn);
	if(ControllingMonsterPawn == nullptr) return EBTNodeResult::Failed;
	
	AEHCharacter* const TargetPlayer = Cast<AEHCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	
	FVector const CurrentLocation = ControllingMonsterPawn->GetActorLocation();
	FVector const TargetLocation = TargetPlayer->GetActorLocation();
	
	float const Distance = FVector::Dist(CurrentLocation, TargetLocation);
	float const AttackRange = ControllingMonsterPawn->AttackRange;
	
	if(Distance <= AttackRange)
		AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsPlayerInAttackRange, true);
	else
		AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsPlayerInAttackRange, false);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}