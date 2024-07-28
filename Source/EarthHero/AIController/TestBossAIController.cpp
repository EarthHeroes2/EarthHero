// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"

ATestBossAIController::ATestBossAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_TestBoss.BT_TestBoss'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
}

void ATestBossAIController::BeginPlay()
{
	Super::BeginPlay();

	//스킬 1 : 2.5초에 1회씩 본인에게 걸려있는 모든 디버프를 해제
	GetWorldTimerManager().SetTimer(TimerHandle1, this, &ATestBossAIController::Cleanser, 2.5f, true, 0.0f);
}

//스킬 1 : 2.5초에 1회씩 본인에게 걸려있는 모든 디버프를 해제
void ATestBossAIController::Cleanser()
{
	//GetBlackBoardComponent()->SetValueAsObject(BlackboardKeys::TargetPlayer, PlayerCharacter);
}