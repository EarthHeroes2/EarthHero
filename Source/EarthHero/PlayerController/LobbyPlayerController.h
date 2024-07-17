// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EarthHero/Widget/LobbyWidget.h"
#include "LobbyPlayerController.generated.h"


class AEHCharacter;

UCLASS()
class EARTHHERO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	ALobbyPlayerController();

	
	TSubclassOf<class UUserWidget> LobbyWidgetClass;
	ULobbyWidget* LobbyWidget;

	AEHCharacter* LobbyCharacter; //게임모드에서 관리하기 실패
	
	bool bHost = false;

	
	UFUNCTION(Server, Reliable)
	void Server_InitSetup();
	
	void ShowLobbyWidget();

public:
	void DestroyCharacter() const;
	void SetCharacter(AEHCharacter* SpawnedCharacter);

	
	UFUNCTION(Client, Reliable)
	void Client_HostAssignment(bool bHostAssignment, bool bAdvertise, int Difficulty);

	UFUNCTION(Server, Reliable)
	void Server_ClientReadyButtonClicked();

	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerNameList(const TArray<FString>& PlayerNameList);

	UFUNCTION(Client, Reliable)
	void Client_UpdateReadyState(const TArray<bool>& PlayerReadyStateArray);

	UFUNCTION(Client, Reliable)
	void Client_SendToDebugMessage(const FString& Message);

	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FText& Text);
	UFUNCTION(Client, Reliable)
	void Client_SendChatMessage(const FText& Text);

	UFUNCTION(Server, Reliable)
	void Server_PlayerKick(int PlayerNumber);

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerCharacter(int ClassType);

	UFUNCTION(Client, Reliable)
	void Client_SelectDefaultCharacter();

	UFUNCTION(Server, Reliable)
	void Server_ChangeAdvertiseState(bool bAdvertise);

	UFUNCTION(Server, Reliable)
	void Server_SetDifficulty(int Difficulty);

	UFUNCTION(Client, Reliable)
	void Client_UpdateDifficulty(int Difficulty);


	UFUNCTION(Server, Reliable)
	void Server_UpdateLobbyPassword(const FString& Password);

	UFUNCTION(Client, Reliable)
	void Client_UpdateLobbyPasswordResult(bool bSuccess);



	void SetHost();
	void UpdateDifficulty(int Difficulty);
};
