// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAIController.h"
#include "BehaviorTree/BehaviorTree.h"

AMeleeAIController::AMeleeAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("AMeleeAIController = %d"), GetNetMode());
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_MeleeEnemy.BT_MeleeEnemy'"));
		
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}

	
	AISightRadius = 500.f;
	AILoseSightRadius = 700.f;
	AIFieldOfView = 90.f;
	AISightAge = 3.f;
	AILastSeenLocation = 900.f;
}