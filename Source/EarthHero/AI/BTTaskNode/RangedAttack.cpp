// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AIController/AIControllerBase.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

URangedAttack::URangedAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type URangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(AIController->GetPawn());
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;

	AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if (TargetPlayer == nullptr) return EBTNodeResult::Failed;

	FVector SpawnNormalVector = (TargetPlayer->GetActorLocation() - ControllingMonster->GetActorLocation()).GetSafeNormal();

	UE_LOG(LogTemp, Log, TEXT("%s %s"), *SpawnNormalVector.ToString(), *SpawnNormalVector.Rotation().ToString())
	
	ControllingMonster->Attack(SpawnNormalVector);
	
	
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}