// Fill out your copyright notice in the Description page of Project Settings.


#include "KeepSafeFlyDistance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UKeepSafeFlyDistance::UKeepSafeFlyDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Keep Safe Fly Distance");
}

EBTNodeResult::Type UKeepSafeFlyDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* const AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	APawn* const ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* const ControllingMonster = Cast<AMonsterBase>(ControllingPawn);
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;
	
	AEHCharacter* const TargetPlayer = Cast<AEHCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	
	FVector const MonsterLocation = ControllingMonster->GetActorLocation();
	FVector const PlayerLocation = TargetPlayer->GetActorLocation();
	
	//방향을 돌려줌
	FRotator const LookAtRotation = (PlayerLocation - MonsterLocation).Rotation();
	ControllingMonster->SetActorRotation(LookAtRotation);

	
	FVector SafeVector;
	float const DistanceToPlayer = FVector::Dist(MonsterLocation, PlayerLocation);
	float const AttackRange = ControllingMonster->AttackRange;

	//사거리 안에 플레이어가 존재하면 도망
	if(AttackRange >= DistanceToPlayer)
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
	FVector const EndLocation = FVector(MonsterLocation.X, MonsterLocation.Y, MonsterLocation.Z - 3000.f);

	UWorld* World = GetWorld();
	if(World && World->LineTraceSingleByChannel(HitResult, MonsterLocation, EndLocation, ECC_Visibility))
	{
		float const DistanceFromGround = MonsterLocation.Z - HitResult.Location.Z;
		
		if(DistanceFromGround < 600) SafeVector.Z = (600 - DistanceFromGround);
		else if(DistanceFromGround > 900) SafeVector.Z = (DistanceFromGround - 900);
	}
	else return EBTNodeResult::Failed;
	
	ControllingMonster->GetMovementComponent()->Velocity = SafeVector;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	
}