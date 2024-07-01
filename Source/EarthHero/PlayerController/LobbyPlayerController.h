// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"


#include "Blueprint/UserWidget.h"
#include <EarthHero/Widget/LobbyWidget.h>

#include "LobbyPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class EARTHHERO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay();

	ALobbyPlayerController();


protected:
	UFUNCTION(Server, Reliable)
	void Server_InitSetup(bool bAdvertise);

public:
	bool bHost = false;

	UFUNCTION(Client, Reliable)
	void Client_HostAssignment();

	UFUNCTION(Server, Reliable)
	void Server_ClientReady();

	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerNameList(const TArray<FString>& PlayerNameList);

	UFUNCTION(Client, Reliable)
	void Client_UpdateReadyState(const TArray<bool>& PlayerReadyStateArray);

	UFUNCTION(Client, Reliable)
	void Client_SendToDebugMessage(const FString& Message);

	

protected:
	void ShowLobbyWidget();

	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	ULobbyWidget* LobbyWidget;
	
};
