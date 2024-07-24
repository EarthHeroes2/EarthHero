// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPatrolPos.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/TestAIController.h"

UFindPatrolPos::UFindPatrolPos(FObjectInitializer const& ObjectInitializer)
{
	//BTTaskNode의 이름
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* Controller = Cast<ATestAIController>(OwnerComp.GetAIOwner());

	APawn* ControllingPawn = Controller->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	
	FVector const CurrentLocation = ControllingPawn->GetActorLocation();
	FNavLocation NextPatrolLocation;

	//다음 순찰 위치 찾고 (현재 주변 1000 주변 임의 지점)
	if (NavSystem->GetRandomPointInNavigableRadius(CurrentLocation, SearchRadius, NextPatrolLocation,nullptr))
	{
		Controller->GetBlackboardComponent()->SetValueAsVector(ATestAIController::TargetLocation, NextPatrolLocation.Location);
	}
	
	//그 다음 이동할 곳을 확인하기 위한 디버그메시지
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"),*NextPatrolLocation.Location.ToString()));
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}