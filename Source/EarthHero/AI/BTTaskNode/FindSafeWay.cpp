// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSafeWay.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

//이 노드는 뒤로 후퇴하지 못할 때 좌측이나 우측으로 도망가기 위함


UFindSafeWay::UFindSafeWay(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Safe Way");
}

EBTNodeResult::Type UFindSafeWay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
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
		FVector const TempSafeDirection = MonsterLocation - PlayerLocation;
		FVector SafeDirection;
		
		int RandomInt = FMath::RandRange(0, 1);
		if(RandomInt == 0)
		{
			SafeDirection = FVector::CrossProduct(TempSafeDirection, FVector::UpVector);
		}
		else
		{
			SafeDirection = -FVector::CrossProduct(TempSafeDirection, FVector::UpVector);
		}

		FVector const SafeLocation = MonsterLocation + SafeDirection.GetSafeNormal() * 150;
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