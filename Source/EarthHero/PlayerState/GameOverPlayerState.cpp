// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverPlayerState.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Enum/Enums.h"
#include "EarthHero/Socket/SocketClient.h"

//copy propertice가 끝나고 데이터를 서버에 저장해야함
void AGameOverPlayerState::UpdatePlayerData()
{
	FString ExtraInfo;
	const FString SteamID = GetUniqueId().ToString();
	
	UE_LOG(LogTemp, Error, TEXT("id = %s"), *SteamID);

	UEHGameInstance* EhGameInstance= Cast<UEHGameInstance>(GetGameInstance());
	if(EhGameInstance)
	{
		switch (EhGameInstance->Difficulty)
		{
		case 1:
			ExtraInfo = SteamID + "|" + FString::FromInt(2);
			break;
		case 2:
			ExtraInfo = SteamID + "|" + FString::FromInt(4);
			break;
		case 3:
			ExtraInfo = SteamID + "|" + FString::FromInt(10);
			break;
		case 4:
			ExtraInfo = SteamID + "|" + FString::FromInt(30);
			break;
		case 5:
			ExtraInfo = SteamID + "|" + FString::FromInt(100);
			break;
		default:
			ExtraInfo = SteamID + "|0";
			UE_LOG(LogTemp, Error, TEXT("Invalid Difficulty : %d"), EhGameInstance->Difficulty);
		}
	}
	
	USocketClient* NewSocket = NewObject<USocketClient>(this);
	if(NewSocket) NewSocket->CreateSocket(Server_UpdatePlayerExp, ExtraInfo);
}