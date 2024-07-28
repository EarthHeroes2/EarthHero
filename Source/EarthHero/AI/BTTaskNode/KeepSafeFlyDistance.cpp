// Fill out your copyright notice in the Description page of Project Settings.


#include "KeepSafeFlyDistance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UKeepSafeFlyDistance::UKeepSafeFlyDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Keep Safe Fly Distance");
}

EBTNodeResult::Type UKeepSafeFlyDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(AIController->GetPawn());
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	
	float AttackRange = ControllingMonster->AttackRange;
	
	FVector const MonsterLocation = ControllingMonster->GetActorLocation();
	FVector const PlayerLocation = TargetPlayer->GetActorLocation();
	
	float const DistanceToPlayer = FVector::Dist(MonsterLocation, PlayerLocation);
	
	//방향을 돌려줌
	FRotator LookAtRotation = (PlayerLocation - MonsterLocation).Rotation();
	ControllingMonster->SetActorRotation(LookAtRotation);


	FVector SafeVector;

	if(AttackRange >= DistanceToPlayer) //사거리 안에 플레이어가 존재하면 도망
	{
		FVector const SafeDirection = MonsterLocation - PlayerLocation; 
		SafeVector = SafeDirection.GetSafeNormal() * (AttackRange - DistanceToPlayer);
	}
	else //사거리 안에 플레이어가 없으면 접근
	{
		FVector const ChaseDirection = PlayerLocation - MonsterLocation; 
		SafeVector = ChaseDirection.GetSafeNormal() * (DistanceToPlayer - AttackRange + 50); //쏘기 좋게 50 더 접근
	}

	//땅과의 일정 거리 유지
	FHitResult HitResult;
	FVector EndLocation = MonsterLocation;
	EndLocation.Z -= 3000.f;

	UWorld* World = GetWorld();
	if(World && World->LineTraceSingleByChannel(HitResult, MonsterLocation, EndLocation, ECC_Visibility))
	{
		float DistanceFromGround = MonsterLocation.Z - HitResult.Location.Z;
		if(DistanceFromGround < 600) SafeVector.Z = (600 - DistanceFromGround);
		else if(DistanceFromGround > 900) SafeVector.Z = (DistanceFromGround - 900);
	}
	else return EBTNodeResult::Failed; //임시


	ControllingMonster->GetMovementComponent()->Velocity = SafeVector;

	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	
}