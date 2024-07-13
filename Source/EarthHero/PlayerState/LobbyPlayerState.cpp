// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

#include "EarthHero/Player/EHPlayerState.h"

//현재 플레이어 스테이트에서 새로 생기는 플레이어 스테이트로 정보 복사
void ALobbyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	UE_LOG(LogTemp, Error, TEXT("Copy Properties - Lobby"));
	
	AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (EHPlayerState)
	{
		EHPlayerState->PlayerClass = PlayerClass;
	}

	EHPlayerState->IsCopyPropertiesEnd = true; //임시

	//EHPlayerState->SetStatComponent(); //copy propertie 끝나자 마자 실행 가능. 다만 컴포넌트 파괴를 나중에 해야할듯? -> 패키징 테스트 때 도전
}