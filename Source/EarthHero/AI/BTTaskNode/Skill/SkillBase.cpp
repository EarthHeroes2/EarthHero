// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

AEHCharacter* USkillBase::GetTargetPlayer(AAIControllerBase* AIController)
{
	AEHCharacter* const TargetPlayer = Cast<AEHCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	return TargetPlayer;
}

FVector USkillBase::GetSpawnNormalVector(AAIControllerBase* AIController, AMonsterBase* ControllingMonster)
{
	AEHCharacter* const TargetPlayer = Cast<AEHCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(BlackboardKeys::TargetPlayer));
	if(TargetPlayer == nullptr) return FVector(0.f, 0.f, 0.f); //잘못된 경우
	
	FVector const MonsterLocation = ControllingMonster->GetActorLocation();
	FVector const PlayerLocation = TargetPlayer->GetActorLocation();
	FVector const SpawnNormalVector = (PlayerLocation - MonsterLocation).GetSafeNormal();
	
	return SpawnNormalVector;
}

