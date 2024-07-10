// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameSession.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void APlayingGameSession::BeginPlay()
{
	Super::BeginPlay();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
			if (SessionSettings)
			{
				FOnlineSessionSetting* NumJoinedPlayersSetting = SessionSettings->Settings.Find("NumberOfJoinedPlayers");
				if(NumJoinedPlayersSetting)
				{
					//세션의 참가 플레이어 수를 가져옴??????????????????????????????????????? (직접 register에서 카운트해주는게 좋나?)
					NumberOfPlayersInSession = FCString::Atoi(*NumJoinedPlayersSetting->Data.ToString());
		
					UE_LOG(LogTemp, Log, TEXT("Receive NumberOfPlayersInSession : %d"), NumberOfPlayersInSession);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No session settings found for session: %s"), *SessionName.ToString());
			}
		}
	}
}
