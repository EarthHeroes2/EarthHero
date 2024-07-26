// Fill out your copyright notice in the Description page of Project Settings.


#include "KeepSafeDistance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/TestAIController.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

//이 노드는 사거리 안일 때 플레이어로부터 안전거리를 유지하기 위함

UKeepSafeDistance::UKeepSafeDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Keep Safe Distance");
}

EBTNodeResult::Type UKeepSafeDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATestAIController* AIController = Cast<ATestAIController>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(AIController->GetPawn());
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;
	
	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	
	float AttackRange = ControllingMonster->AttackRange;
	
	FVector const MonsterLocation = AIController->GetPawn()->GetActorLocation();
	FVector const PlayerLocation = TargetPlayer->GetActorLocation();
	
	float const DistanceToPlayer = FVector::Dist(MonsterLocation, PlayerLocation);

	//플레이어를 바라보며 이동하기 위해
	//ai컨트롤러 틱을 잠시 비활성화
	AIController->SetActorTickEnabled(false);
	//그리고 방향을 돌려줌
	FRotator LookAtRotation = (PlayerLocation - MonsterLocation).Rotation();
	LookAtRotation.Roll = 0.f;
	LookAtRotation.Pitch = 0.f;
	ControllingMonster->SetActorRotation(LookAtRotation);

	if(AttackRange >= DistanceToPlayer) //사거리 안에 플레이어가 존재하면 도망
	{
		FVector const SafeDirection = MonsterLocation - PlayerLocation; 
		FVector const SafeLocation = MonsterLocation + SafeDirection.GetSafeNormal() * (AttackRange - DistanceToPlayer);
	
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, SafeLocation);
	}
	else //사거리 안에 플레이어가 없으면 접근
	{
		FVector const ChaseDirection = PlayerLocation - MonsterLocation; 
		FVector const SafeLocation = MonsterLocation + ChaseDirection.GetSafeNormal() * (DistanceToPlayer - AttackRange);
	
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, SafeLocation);
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	
}