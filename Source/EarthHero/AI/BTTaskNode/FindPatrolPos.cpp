// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPatrolPos.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/TestAIController.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"

UFindPatrolPos::UFindPatrolPos(FObjectInitializer const& ObjectInitializer)
{
	//BTTaskNode의 이름
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* AIController = Cast<ATestAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	APawn* ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	
	FVector const CurrentLocation = ControllingPawn->GetActorLocation();
	FNavLocation NextPatrolLocation;

	//다음 순찰 위치 찾고 (현재 주변 1000 주변 임의 지점)
	if (NavSystem->GetRandomPointInNavigableRadius(CurrentLocation, SearchRadius, NextPatrolLocation,nullptr))
	{
		//TargetLocation에 위치 값 저장
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, NextPatrolLocation.Location);
	}

	//
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}