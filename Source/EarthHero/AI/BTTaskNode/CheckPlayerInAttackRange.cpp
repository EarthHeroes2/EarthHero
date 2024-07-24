// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPlayerInAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/TestAIController.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

UCheckPlayerInAttackRange::UCheckPlayerInAttackRange(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Check Player In Attack Range");
}

EBTNodeResult::Type UCheckPlayerInAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* AIController = Cast<ATestAIController>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	APawn* ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	FVector const CurrentLocation = ControllingPawn->GetActorLocation();
	FVector const TargetLocation = TargetPlayer->GetActorLocation();
	float const Distance = FVector::Dist(CurrentLocation, TargetLocation);

	AMonsterBase* ControllingMonsterPawn = Cast<AMonsterBase>(ControllingPawn);
	if(ControllingMonsterPawn == nullptr) return EBTNodeResult::Failed;
	float const AttackRange = ControllingMonsterPawn->AttackRange;
	
	if(Distance <= AttackRange)
		AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsPlayerInAttackRange, true);
	else
		AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsPlayerInAttackRange, false);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}