// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameSession.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"

class APlayingGameMode;

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

void APlayingGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
	Super::NotifyLogout(ExitingPlayer); //여기있어도 되려나?

	if(IsRunningDedicatedServer())
	{
		// 게임모드의 관리 리스트에서 제거
		APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
		if(PlayingGameMode)
		{
			const AEHPlayerController* ConstExitingEHPlayerController = Cast<AEHPlayerController>(ExitingPlayer);
			if(ConstExitingEHPlayerController)
			{
				AEHPlayerController* ExitingEHPlayerController = const_cast<AEHPlayerController*>(ConstExitingEHPlayerController);
				if(ExitingEHPlayerController) PlayingGameMode->PlayerLogOut(ExitingEHPlayerController);
			}
		}
	}
}
