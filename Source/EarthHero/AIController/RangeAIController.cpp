// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeAIController.h"
#include "BehaviorTree/BehaviorTree.h"

ARangeAIController::ARangeAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Log, TEXT("ARangeAIController = %d"), GetNetMode());
	if(IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_RangedEnemy.BT_RangedEnemy'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
}