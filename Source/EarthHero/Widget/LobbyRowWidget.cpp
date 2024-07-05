// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyRowWidget.h"

#include "Components/Button.h"


bool ULobbyRowWidget::Initialize()
{
	Super::Initialize();

	//너무빨리 버튼을 생성해주나?
	LobbyJoin_Btn->OnClicked.AddDynamic(this, &ULobbyRowWidget::JoinClicked);
	
	return true;
}

void ULobbyRowWidget::UpdateLobbyInfo()
{
	
}

void ULobbyRowWidget::JoinClicked()
{
	
}



