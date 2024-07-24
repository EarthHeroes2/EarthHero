// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TestAIController.generated.h"

class UAISenseConfig_Sight;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class EARTHHERO_API ATestAIController : public AAIController
{
	GENERATED_BODY()

	ATestAIController(FObjectInitializer const& ObjectInitializer);

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
	
private:
	float AISightRadius = 500.f;
	float AILoseSightRadius = 200.f;
	float AIFieldOfView = 90.f; //이거 캐릭 정면 기준 좌,우로 2배가 되네?
	float AISightAge = 4.f;
	float AILastSeenLocation = 900.f;
};
