// Fill out your copyright notice in the Description page of Project Settings.

#include "HellGolemAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"

AHellGolemAIController::AHellGolemAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_HellGolem.BT_HellGolem'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
	
	AISightRadius = 3000.f;
	AILoseSightRadius = 3700.f;
	AIFieldOfView = 180.f;
	AISightAge = 3.f;
	AILastSeenLocation = 900.f;
}

void AHellGolemAIController::Skill1()
{
}

void AHellGolemAIController::Skill2()
{
}
