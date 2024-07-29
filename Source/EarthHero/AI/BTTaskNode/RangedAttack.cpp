// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

URangedAttack::URangedAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type URangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	FVector const SpawnNormalVector = (PlayerLocation - MonsterLocation).GetSafeNormal();

	//공격을 날릴 방향을 넘겨줌
	ControllingMonster->Attack(SpawnNormalVector);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}