// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyRowWidget.h"

#include "MainMenuWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"



bool ULobbyRowWidget::Initialize()
{
	Super::Initialize();
	
	LobbyJoin_Btn->OnClicked.AddDynamic(this, &ULobbyRowWidget::JoinClicked);
	
	return true;
}

void ULobbyRowWidget::UpdateLobbyInfo(FOnlineSessionSearchResult Lobby)
{
	LobbyInfo = Lobby;
	
	FString LobbyName;
	bool bKeyValueFound = LobbyInfo.Session.SessionSettings.Get("LobbyName", LobbyName);

	if(bKeyValueFound) LobbyName_Tb->SetText(FText::FromString(LobbyName));
	else LobbyName_Tb->SetText(FText::FromString("Earth Hero"));

	
	int NumberOfJoinedPlayers;

	FText MaxNumberOfPlayers = FText::AsNumber(LobbyInfo.Session.SessionSettings.NumPublicConnections);
	bool bKeyValueFound = LobbyInfo.Session.SessionSettings.Get("NumberOfJoinedPlayers", NumberOfJoinedPlayers);
	
	if(bKeyValueFound)
		PlayerCount_Tb->SetText(FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(NumberOfJoinedPlayers), MaxNumberOfPlayers));

	
	Ping_Tb->SetText(FText::Format(FText::FromString("{0}ms"), LobbyInfo.PingInMs));
}

void ULobbyRowWidget::JoinClicked()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			FString ConnectString;
			
			if (Session->GetResolvedConnectString(LobbyInfo, NAME_GamePort, ConnectString))
			{
				FOnlineSessionSearchResult* SessionToJoin = &LobbyInfo;
				if (SessionToJoin)
				{
					if (MainMenuWidget)
					{
						MainMenuWidget->ConnectString = ConnectString;
						MainMenuWidget->SessionToJoin = SessionToJoin;

						MainMenuWidget->LeaveSession("JoinSelectedLobby");
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ParentWidget(MainMenuWidget) is not valid"));
					}
				}
			}
		}
	}
}



