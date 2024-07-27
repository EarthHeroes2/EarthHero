// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingAIController.h"
#include "BehaviorTree/BehaviorTree.h"

AFlyingAIController::AFlyingAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("FlyingAIController = %d"), GetNetMode());
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_FlyingEnemy.BT_FlyingEnemy'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
}
