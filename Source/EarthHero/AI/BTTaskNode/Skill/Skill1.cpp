// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill1.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

USkill1::USkill1(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Skill 1");
}

EBTNodeResult::Type USkill1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIControllerBase* const AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	APawn* const ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* const ControllingMonster = Cast<AMonsterBase>(ControllingPawn);
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;

	
	switch (ControllingMonster->BossNumber)
	{
		case MainBoss:
			break;
		case MidBoss1:
			ControllingMonster->Skill1();
			break;
		case MidBoss2:
			ControllingMonster->Skill1();
			break;
		case MidBoss3:
			break;
		case MidBoss4:
			break;
		case MidBoss5:
			break;
		case MidBoss6:
			break;
		default:
			return EBTNodeResult::Failed;
	}
	
	AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsSkill1Ready, false);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}