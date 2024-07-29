// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss6AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"

AMidBoss6AIController::AMidBoss6AIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_MidBoss6.BT_MidBoss6'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
	
	AISightRadius = 500.f;
	AILoseSightRadius = 700.f;
	AIFieldOfView = 90.f;
	AISightAge = 3.f;
	AILastSeenLocation = 900.f;
}

void AMidBoss6AIController::BeginPlay()
{
	Super::BeginPlay();


	GetWorldTimerManager().SetTimer(TimerHandle1, this, &ThisClass::Skill1, 2.5f, true, 0.f);
	
	GetWorldTimerManager().SetTimer(TimerHandle2, this, &ThisClass::Skill2, 10.f, true, 0.f);
}

void AMidBoss6AIController::Skill1()
{
	GetBlackBoardComponent()->SetValueAsBool(BlackboardKeys::IsSkill1Ready, true);
}

void AMidBoss6AIController::Skill2()
{
	GetBlackBoardComponent()->SetValueAsBool(BlackboardKeys::IsSkill2Ready, true);
}