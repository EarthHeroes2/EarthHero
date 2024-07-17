// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyRowWidget.h"

#include "MainMenuWidget.h"
#include "OnlineSessionSettings.h"
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
	
	bool bKeyValueFound = LobbyInfo.Session.SessionSettings.Get("Advertise", bAdvertise);

	if(bKeyValueFound)
	{
		if(!bAdvertise)
		{
			SetColorAndOpacity(FLinearColor::Yellow);
		}
	}
	
	FString LobbyName;
	bKeyValueFound = LobbyInfo.Session.SessionSettings.Get("LobbyName", LobbyName);

	if(bKeyValueFound) LobbyName_Tb->SetText(FText::FromString(LobbyName));
	else LobbyName_Tb->SetText(FText::FromString("Earth Hero"));
	
	int NumberOfJoinedPlayers;

	FText MaxNumberOfPlayers = FText::AsNumber(LobbyInfo.Session.SessionSettings.NumPublicConnections);
	bKeyValueFound = LobbyInfo.Session.SessionSettings.Get("NumberOfJoinedPlayers", NumberOfJoinedPlayers);
	
	if(bKeyValueFound)
		PlayerCount_Tb->SetText(FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(NumberOfJoinedPlayers), MaxNumberOfPlayers));

	
	Ping_Tb->SetText(FText::Format(FText::FromString("{0}ms"), LobbyInfo.PingInMs));
}

void ULobbyRowWidget::JoinClicked()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->ServerRowClicked(LobbyInfo, bAdvertise);
	}
}