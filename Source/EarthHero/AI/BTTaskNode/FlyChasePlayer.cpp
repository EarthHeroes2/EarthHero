// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UFlyChasePlayer::UFlyChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Fly Chase Player");
}

EBTNodeResult::Type UFlyChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(AIController->GetPawn());
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	FVector const MonsterLocation = ControllingMonster->GetActorLocation();
	FVector const PlayerLocation = TargetPlayer->GetActorLocation();
	
	float const DistanceToPlayer = FVector::Dist(MonsterLocation, PlayerLocation);
	
	//방향을 돌려줌
	FRotator LookAtRotation = (PlayerLocation - MonsterLocation).Rotation();
	ControllingMonster->SetActorRotation(LookAtRotation);
	
	FVector ChaseDirection = PlayerLocation - MonsterLocation; 

	//땅과 너무 가까우면 안됨
	FHitResult HitResult;
	FVector EndLocation = MonsterLocation;
	EndLocation.Z -= 3000.f;

	UWorld* World = GetWorld();
	if(World && World->LineTraceSingleByChannel(HitResult, MonsterLocation, EndLocation, ECC_Visibility))
	{
		float DistanceFromGround = MonsterLocation.Z - HitResult.Location.Z;
		if(DistanceFromGround < 150) ChaseDirection.Z = (150 - DistanceFromGround);
	}
	else return EBTNodeResult::Failed; //임시
	
	ControllingMonster->GetMovementComponent()->Velocity = ChaseDirection;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}