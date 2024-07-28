// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingMeleeAIController.h"
#include "BehaviorTree/BehaviorTree.h"

AFlyingMeleeAIController::AFlyingMeleeAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("FlyingMeleeAIController = %d"), GetNetMode());
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_FlyingMeleeEnemy.BT_FlyingMeleeEnemy'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
}

