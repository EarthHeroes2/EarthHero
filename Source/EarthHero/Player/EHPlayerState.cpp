// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerState.h"

#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/StatComponent.h"

AEHPlayerState::AEHPlayerState()
{
}

//현재 플레이어 스테이트에서 새로 생기는 플레이어 스테이트로 정보 복사
void AEHPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	
	AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (EHPlayerState)
	{
		EHPlayerState->PlayerClass = PlayerClass;
	}
}
