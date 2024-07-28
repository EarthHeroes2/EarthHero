// Fill out your copyright notice in the Description page of Project Settings.


#include "FindFlyingPatrolPos.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/Character/Monster/DummyFlyingMonster.h"
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

	ADummyFlyingMonster* ControllingFlyingMonster = Cast<ADummyFlyingMonster>(ControllingPawn);
	if (ControllingFlyingMonster == nullptr) return EBTNodeResult::Failed;

	
	FVector Location;
	
	Location.X += FMath::FRandRange(-400.f, 400.f);
	Location.Y += FMath::FRandRange(-400.f, 400.f);
	Location.Z += FMath::FRandRange(-40.f, 40.f); //지상과 너무 가깝지 않게 추가적인 작업 필요

	ControllingPawn->SetActorRotation(Location.Rotation());
	ControllingPawn->GetMovementComponent()->Velocity = Location;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}