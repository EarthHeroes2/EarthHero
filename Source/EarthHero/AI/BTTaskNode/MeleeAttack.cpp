// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"

UMeleeAttack::UMeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("666666666666666666666666666666"));
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}