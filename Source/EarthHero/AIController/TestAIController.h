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
	
	UBlackboardComponent* GetBlackBoardComponent() const;


	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& UpdatedActors);
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
	


	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* BlackBoardComponent;

	UAISenseConfig_Sight* SightConfig;

	UBehaviorTree* BehavirTree;


	static const FName HomePosKey;
	static const FName TargetLocation;
	
	
	
	float AISightRadius = 500.f;
	float AILoseSightRadius = 50.f;
	float AIFieldOfView = 90.f;
	float AISightAge = 5.f;
	float AILastSeenLocation = 900.f;
};
