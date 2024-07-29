// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EarthHero/Enum/Enums.h"
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

protected:
	AAIControllerBase(FObjectInitializer const& ObjectInitializer);
	virtual void BeginPlay() override;
	
private:
	virtual void OnPossess(APawn* InPawn) override;
	
	void UpdatePerceptionSystem();
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
	
	
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* BlackBoardComponent;
	
protected:
	UBlackboardComponent* GetBlackBoardComponent() const;
	
	UBehaviorTree* BehavirTree;
	
	void SetPerceptionSystem();
	
private:
	UAISenseConfig_Sight* SightConfig;
protected:
	//임시로 만들어둠 - 박정익
	//자식에서 상속받아서 값 바꾸어주세용
	//원거리 몬스터인 경우 사거리가 시야보다 더 넓게 해주세요
	float AttackRange = 100.0f;

	//시야 거리, 목표 상실 거리, 시야각, 감각을 잃는 기간, 마지막으로 감지된 객체의 위치 탐지 성공 거리
	//기본값은 임시값
	float AISightRadius = 400.f;
	float AILoseSightRadius = 500.f;
	float AIFieldOfView = 90.f; //시야각인데 *2가 됨
	float AISightAge = 4.f;
	float AILastSeenLocation = 600.f;

public:
	void AttackedPlayer(AActor* AttackedPlayer);
};
