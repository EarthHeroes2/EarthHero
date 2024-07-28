// Fill out your copyright notice in the Description page of Project Settings.


#include "KeepSafeDistance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

//이 노드는 사거리 안일 때 플레이어로부터 안전거리를 유지하기 위함

UKeepSafeDistance::UKeepSafeDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Keep Safe Distance");
}

EBTNodeResult::Type UKeepSafeDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	
	//플레이어를 바라보며 이동하기 위해 ai컨트롤러 틱을 비활성화하고 방향 전환
	AIController->SetActorTickEnabled(false);
	
	FRotator LookAtRotation = (PlayerLocation - MonsterLocation).Rotation();
	LookAtRotation.Roll = 0.f;
	LookAtRotation.Pitch = 0.f;
	ControllingMonster->SetActorRotation(LookAtRotation);

	
	float const DistanceToPlayer = FVector::Dist(MonsterLocation, PlayerLocation);
	float const AttackRange = ControllingMonster->AttackRange;
	
	//사거리 안에 플레이어가 존재하면 도망
	if(AttackRange >= DistanceToPlayer)
	{
		FVector const SafeDirection = MonsterLocation - PlayerLocation; 
		FVector const SafeLocation = MonsterLocation + SafeDirection.GetSafeNormal() * (AttackRange - DistanceToPlayer);
	
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, SafeLocation);
	}
	else //사거리 안에 플레이어가 없으면 접근
	{
		FVector const ChaseDirection = PlayerLocation - MonsterLocation; 
		FVector const SafeLocation = MonsterLocation + ChaseDirection.GetSafeNormal() * (DistanceToPlayer - AttackRange + 50); //쏘기 좋게 50 더 접근
	
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, SafeLocation);
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}