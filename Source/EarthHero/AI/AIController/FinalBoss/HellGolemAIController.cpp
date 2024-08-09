// Fill out your copyright notice in the Description page of Project Settings.

#include "HellGolemAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EarthHero/AI/BlackBoard/BlackBoardKeys.h"
#include "EarthHero/GameMode/PlayingGameMode.h"

AHellGolemAIController::AHellGolemAIController(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	if(GetNetMode() != NM_Client)
	{
		static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Ai/BT_HellGolem.BT_HellGolem'"));
		if (BTObject.Succeeded()) BehavirTree = BTObject.Object;
	}
	
	AISightRadius = 3000.f;
	AILoseSightRadius = 3700.f;
	AIFieldOfView = 180.f;
	AISightAge = 3.f;
	AILastSeenLocation = 900.f;
}

void AHellGolemAIController::BeginPlay()
{
	Super::BeginPlay();

	APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayingGameMode)
	{
		if (PlayingGameMode->IsDebugMode)
		{
			if (GetNetMode() == NM_ListenServer)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &AHellGolemAIController::SetSkill1CoolTime, Skill1CoolTime, false, 1);
			}
		}
		else
		{
			if (IsRunningDedicatedServer())
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &AHellGolemAIController::SetSkill1CoolTime, Skill1CoolTime, false, 1);
			}
		}
	}
}

void AHellGolemAIController::Skill1()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &AHellGolemAIController::SetSkill1CoolTime, Skill1CoolTime, true);
	UE_LOG(LogClass, Error, TEXT("HellGollem : Skill 1 Activated"));
}

void AHellGolemAIController::Skill2()
{
}

void AHellGolemAIController::Skill3()
{
}

void AHellGolemAIController::Skill4()
{
}

void AHellGolemAIController::SetSkill1CoolTime()
{
	GetBlackBoardComponent()->SetValueAsBool(BlackboardKeys::IsSkill1Ready, true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle1);
}

void AHellGolemAIController::SetSkill2CoolTime()
{
}

void AHellGolemAIController::SetSkill3CoolTime()
{
}

void AHellGolemAIController::SetSkill4CoolTime()
{
}

