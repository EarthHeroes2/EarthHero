// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedAttack.h"

#include "EarthHero/AIController/TestAIController.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

URangedAttack::URangedAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Ranged Attack");
}

EBTNodeResult::Type URangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack to player!!!!!!!"));

	ATestAIController* AIController = Cast<ATestAIController>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(AIController->GetPawn());
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;

	
	//AActor* const TargetPlayer = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	//if (TargetPlayer == nullptr) return EBTNodeResult::Failed;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}