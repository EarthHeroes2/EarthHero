// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIControllerBase.generated.h"

class UAISenseConfig_Sight;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class EARTHHERO_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

	AAIControllerBase(FObjectInitializer const& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;


	
	void SetPerceptionSystem();
	void UpdatePerceptionSystem();

	UBlackboardComponent* GetBlackBoardComponent() const;
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
	
	
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* BlackBoardComponent;

	UBehaviorTree* BehavirTree;
	UAISenseConfig_Sight* SightConfig;
};
