// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UFlyChasePlayer::UFlyChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Fly Chase Player");
}

EBTNodeResult::Type UFlyChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	FVector ChaseDirection = PlayerLocation - MonsterLocation;

	//플레이어를 바라보며
	ControllingMonster->SetActorRotation(ChaseDirection.Rotation());
	
	//땅과 너무 가까우면 안됨
	FHitResult HitResult;
	FVector const EndLocation = FVector(MonsterLocation.X, MonsterLocation.Y, MonsterLocation.Z - 3000.f);

	UWorld* World = GetWorld();
	if(World && World->LineTraceSingleByChannel(HitResult, MonsterLocation, EndLocation, ECC_Visibility))
	{
		float const DistanceFromGround = MonsterLocation.Z - HitResult.Location.Z;
		if(DistanceFromGround < 200) ChaseDirection.Z = (200 - DistanceFromGround);
	}
	else return EBTNodeResult::Failed; //임시
	
	ControllingMonster->GetMovementComponent()->Velocity = ChaseDirection;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}