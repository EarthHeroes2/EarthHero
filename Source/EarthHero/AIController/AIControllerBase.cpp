// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"

AAIControllerBase::AAIControllerBase(FObjectInitializer const& ObjectInitializer)
{
	if(IsRunningDedicatedServer())
	{
		//비헤이비어트리를 찾고
		//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_MeleeEnemy.BT_MeleeEnemy'"));
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_RangedEnemy.BT_RangedEnemy'"));
	
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
		BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
		BlackBoardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));

		//Perception초기화
		SetPerceptionSystem();
	}
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsRunningDedicatedServer())
	{
		//비헤이비어 트리 실행
		RunBehaviorTree(BehavirTree);
		//위와 무슨 차이지?
		BehaviorTreeComponent->StartTree(*BehavirTree);
	}
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UpdatePerceptionSystem();
	
	if (BlackBoardComponent)
	{
		//비헤이비어 트리에 연결된 블랙보드로 초기화
		BlackBoardComponent->InitializeBlackboard(*BehavirTree->BlackboardAsset);
	}
}

UBlackboardComponent* AAIControllerBase::GetBlackBoardComponent() const
{
	return BlackBoardComponent;
}

void AAIControllerBase::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	//플레이어 캐릭터만을 걸러내고
	AEHCharacter* PlayerCharacter = Cast<AEHCharacter>(Actor);
	if (PlayerCharacter)
	{
		//감지 성공 시
		//블랙보드의 CanSeePlayer에 true
		GetBlackBoardComponent()->SetValueAsBool(BlackboardKeys::CanSeePlayer, Stimulus.WasSuccessfullySensed());
		//해당 플레이어를 쫒을 목표로 설정
		GetBlackBoardComponent()->SetValueAsObject(BlackboardKeys::TargetPlayer, PlayerCharacter);
	}
}

//플레이어한테 맞으면 그를 대상으로 삼음
void AAIControllerBase::AttackedPlayer(AActor* AttackedPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("AttackedPlayer AttackedPlayer  AttackedPlayer"));
	//다만 목표대상이 없을 때만 때린놈을 대상으로 삼음
	bool bCanSeePlayer = GetBlackBoardComponent()->GetValueAsBool(BlackboardKeys::CanSeePlayer);
	if(!bCanSeePlayer)
	{
		GetBlackBoardComponent()->SetValueAsBool(BlackboardKeys::CanSeePlayer, true);
		GetBlackBoardComponent()->SetValueAsObject(BlackboardKeys::TargetPlayer, AttackedPlayer);
	}
}

//기본 시야 설정
void AAIControllerBase::SetPerceptionSystem()
{
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	//적, 중립, 아군 전부 감지
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //주된 감각 = 시야
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetDetected); //감각에 감지 시
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIControllerBase::UpdatePerceptionSystem()
{
	AMonsterBase* ControllingMonster = Cast<AMonsterBase>(GetPawn());
	if(ControllingMonster && SightConfig)
	{
		//시야 거리, 목표 상실 거리, 시야각, 감각을 잃는 기간, 마지막으로 감지된 객체의 위치 탐지 성공 거리?
		SightConfig->SightRadius = ControllingMonster->AISightRadius;
		SightConfig->LoseSightRadius = ControllingMonster->AILoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = ControllingMonster->AIFieldOfView;
		SightConfig->SetMaxAge(ControllingMonster->AISightAge);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = ControllingMonster->AILastSeenLocation;
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}