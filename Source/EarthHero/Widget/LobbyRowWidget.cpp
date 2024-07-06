// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyRowWidget.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


bool ULobbyRowWidget::Initialize()
{
	Super::Initialize();

	//너무빨리 버튼을 생성해주나?
	LobbyJoin_Btn->OnClicked.AddDynamic(this, &ULobbyRowWidget::JoinClicked);
	
	return true;
}

void ULobbyRowWidget::UpdateLobbyInfo(FOnlineSessionSearchResult Lobby)
{
	FString NumberOfJoinedPlayers;
	bool bKeyValueFound = Lobby.Session.SessionSettings.Get("NumberOfJoinedPlayers", NumberOfJoinedPlayers);

	if(bKeyValueFound)
	{
		PlayerCount_Tb->SetText(FText::FromString(NumberOfJoinedPlayers));
	}
	LobbyName_Tb->SetText(FText::FromString(Lobby.GetSessionIdStr()));
	Ping_Tb->SetText(FText::AsNumber(Lobby.PingInMs));
}

void ULobbyRowWidget::JoinClicked()
{
	
}



