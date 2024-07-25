// Fill out your copyright notice in the Description page of Project Settings.


#include "KeepSafeDistance.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/TestAIController.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

//이 노드는 사거리 안일 때 플레이어로부터 안전거리를 유지하기 위함

UKeepSafeDistance::UKeepSafeDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Keep Safe Distance");
}

EBTNodeResult::Type UKeepSafeDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* AIController = Cast<ATestAIController>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(AIController->GetPawn());
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	
	float AttackRange = ControllingMonster->AttackRange;
	
	FVector const MonsterLocation = AIController->GetPawn()->GetActorLocation();
	FVector const PlayerLocation = TargetPlayer->GetActorLocation();
	
	float const DistanceToPlayer = FVector::Dist(MonsterLocation, PlayerLocation);
	
	FVector const SafeDirection = MonsterLocation - PlayerLocation; 
	FVector const SafeLocation = MonsterLocation + SafeDirection.GetSafeNormal() * (AttackRange - DistanceToPlayer);

	UE_LOG(LogTemp, Warning, TEXT("%s   %s   %s"), *MonsterLocation.ToString(), *PlayerLocation.ToString(), *SafeLocation.ToString());
	
	AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, SafeLocation);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}