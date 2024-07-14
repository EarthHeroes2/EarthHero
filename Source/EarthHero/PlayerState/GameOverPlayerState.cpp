// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverPlayerState.h"
#include "EarthHero/GameMode/GameOverGameMode.h"

void AGameOverPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if(GetNetMode() != NM_Client)
	{
		
		GetWorldTimerManager().SetTimer(SetStatComponentTimerHandle, this, &ThisClass::TestFunc, 4.5f,false);
	}
}


//copy propertice가 끝나고 데이터를 서버에 저장해야함
void AGameOverPlayerState::TestFunc()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();
	if (GameMode)
	{
		AGameOverGameMode* GameOverGameMode = Cast<AGameOverGameMode>(GameMode);
		if(GameOverGameMode)
		{
			//플레이어 정보 저장



			
		}
	}
}