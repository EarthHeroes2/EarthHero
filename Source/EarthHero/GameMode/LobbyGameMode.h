// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();
	int ReadyCount = 0;
	TArray<FString> PlayerNameArray;
	TArray<bool> PlayerReadyStateArray;

public:
	ALobbyGameMode();

	TArray<ALobbyPlayerController*> LobbyPlayerControllerArray;

	void TogglePlayerReady(APlayerController* Player);
	void UpdatePlayerNameyListAndReadyState();
	void UpdatePlayerReadyState();
	void AddPlayerReadyState(APlayerController* Player);
	bool PressGameStartButton();

	void SendChatMessage(const FText& Text);


};
