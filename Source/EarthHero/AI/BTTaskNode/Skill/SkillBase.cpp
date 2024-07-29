// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"

AEHCharacter* USkillBase::GetTargetPlayer(AAIControllerBase* AIController)
{
	AEHCharacter* const TargetPlayer = Cast<AEHCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	return TargetPlayer;
}
