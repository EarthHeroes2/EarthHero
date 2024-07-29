// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss1AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"

AMidBoss1AIController::AMidBoss1AIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_MidBoss1.BT_MidBoss1'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
	
	AISightRadius = 500.f;
	AILoseSightRadius = 700.f;
	AIFieldOfView = 90.f;
	AISightAge = 3.f;
	AILastSeenLocation = 900.f;
}

void AMidBoss1AIController::BeginPlay()
{
	Super::BeginPlay();

	//스킬 1 : 2.5초에 1회씩 본인에게 걸려있는 모든 디버프를 해제
	GetWorldTimerManager().SetTimer(TimerHandle1, this, &AMidBoss1AIController::Cleanser, 2.5f, true, 0.0f);
}

//스킬 1 : 2.5초에 1회씩 본인에게 걸려있는 모든 디버프를 해제
void AMidBoss1AIController::Cleanser()
{
	GetBlackBoardComponent()->SetValueAsBool(BlackboardKeys::IsSkill1Ready, true);
}