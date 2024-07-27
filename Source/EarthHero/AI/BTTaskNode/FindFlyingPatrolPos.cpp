// Fill out your copyright notice in the Description page of Project Settings.


#include "FindFlyingPatrolPos.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "GameFramework/PawnMovementComponent.h"

UFindFlyingPatrolPos::UFindFlyingPatrolPos(FObjectInitializer const& ObjectInitializer)
{
	//BTTaskNode의 이름
	NodeName = TEXT("Find Flying Patrol Pos");
}

EBTNodeResult::Type UFindFlyingPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	APawn* ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
	
	//본래 회전 상태로 복원
	//AIController->SetActorTickEnabled(true);
	//ControllingPawn->SetActorRotation(FRotator(0.f, 270.f, 0.f));

	FVector Location;
	
	Location.X += FMath::FRandRange(0.f, 300.f);
	Location.Y += FMath::FRandRange(0.f, 300.f);
	Location.Z += FMath::FRandRange(0.f, 30.f);
	
	//TargetLocation에 위치 값 저장
	//AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, Location);

	//ControllingPawn->GetMovementComponent()->Velocity = Location;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}