// Fill out your copyright notice in the Description page of Project Settings.


#include "FindFlyingPatrolPos.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "GameFramework/PawnMovementComponent.h"

UFindFlyingPatrolPos::UFindFlyingPatrolPos(FObjectInitializer const& ObjectInitializer)
{
	//BTTaskNode의 이름
	NodeName = TEXT("Find Flying Patrol Pos");
}

EBTNodeResult::Type UFindFlyingPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* const AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	APawn* const ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;
	
	FVector AddLocation;
	AddLocation.X = FMath::FRandRange(-400.f, 400.f);
	AddLocation.Y = FMath::FRandRange(-400.f, 400.f);
	AddLocation.Z = FMath::FRandRange(-50.f, 50.f); //지상과 너무 가깝지 않게 추가적인 작업 필요

	//땅과의 일정 거리 유지
	FHitResult HitResult;
	FVector const MonsterLocation = ControllingPawn->GetActorLocation();
	FVector const EndLocation = FVector(MonsterLocation.X, MonsterLocation.Y, MonsterLocation.Z - 3000.f);

	UWorld* World = GetWorld();
	if(World && World->LineTraceSingleByChannel(HitResult, MonsterLocation, EndLocation, ECC_Visibility))
	{
		float const DistanceFromGround = MonsterLocation.Z - HitResult.Location.Z;
		
		if(DistanceFromGround < 600) AddLocation.Z = FMath::FRandRange(0.f, 50.f);
		else if(DistanceFromGround > 900) AddLocation.Z = FMath::FRandRange(-50.f, 0.f);
	}
	else return EBTNodeResult::Failed; //임시
	
	ControllingPawn->SetActorRotation(AddLocation.Rotation());
	ControllingPawn->GetMovementComponent()->Velocity = AddLocation;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}