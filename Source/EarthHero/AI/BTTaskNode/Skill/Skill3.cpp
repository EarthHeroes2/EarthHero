// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill3.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

USkill3::USkill3(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Skill 3");
}

EBTNodeResult::Type USkill3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* const AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	if(AIController == nullptr) return EBTNodeResult::Failed;

	APawn* const ControllingPawn = AIController->GetPawn();
	if (ControllingPawn == nullptr) return EBTNodeResult::Failed;

	AMonsterBase* const ControllingMonster = Cast<AMonsterBase>(ControllingPawn);
	if(ControllingMonster == nullptr) return EBTNodeResult::Failed;
	
	AEHCharacter* TargetPlayer;
	//FVector SpawnNormalVector;
	
	switch (ControllingMonster->BossNumber)
	{
	case MainBoss:
		ControllingMonster->Skill3();
		break;
	case MidBoss1:
		ControllingMonster->Skill3();
		break;
	case MidBoss2:
		if((TargetPlayer = GetTargetPlayer(AIController)) == nullptr) return EBTNodeResult::Failed;
		ControllingMonster->Skill3(TargetPlayer);
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
	
	AIController->GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsSkill3Ready, false);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
