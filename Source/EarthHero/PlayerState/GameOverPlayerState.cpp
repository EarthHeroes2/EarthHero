// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverPlayerState.h"
#include "EarthHero/GameMode/GameOverGameMode.h"
#include "EarthHero/Socket/SocketClient.h"

void AGameOverPlayerState::BeginPlay()
{
	Super::BeginPlay();
}


//copy propertice가 끝나고 데이터를 서버에 저장해야함
void AGameOverPlayerState::TestFunc()
{
	/*
	*난이도 1 클리어 (입문) : 2pt.
	난이도 2 클리어 (일반) : 4pt.
	난이도 3 클리어 (숙련) : 10pt.
	난이도 4 클리어 (도전) : 30pt.
	난이도 5 클리어 (엔드) : 100pt.
	*/
	if(!IsRunningDedicatedServer()) UE_LOG(LogTemp, Error, TEXT("???????????????????????????????????????????"));

	FString ExtraInfo;
	const FString SteamID = GetUniqueId().ToString();
	
	UE_LOG(LogTemp, Error, TEXT("id = %s"), *SteamID);
	
	//난이도에 따른 경험치 (임시)
	ExtraInfo = SteamID + "|" + FString::FromInt(10);
	
	//플레이어 정보 저장
	USocketClient* NewSocket = NewObject<USocketClient>(this);
	if(NewSocket) NewSocket->CreateSocket("UpdatePlayerExp", ExtraInfo);
}