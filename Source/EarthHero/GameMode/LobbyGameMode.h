// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include "LobbyGameMode.generated.h"

/**
 * 
 */
class AEHShooter;

UCLASS()
class EARTHHERO_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	int ReadyCount = 0;
	
	TArray<FVector> SpawnLocations;
	
	TArray<FString> PlayerNameArray;
	TArray<bool> PlayerReadyStateArray;
	TArray<EClassType> PlayerClassArray;
	TArray<int> PlayerSpotArray;
	
	TArray<bool> bSpotUsedArray;
	

	TArray<TSubclassOf<ACharacter>> CharacterClasses;
	//TSubclassOf<class AEHWarrior> EHWarriorClass;
	//TSubclassOf<class AEHMechanic> EHMechanicClass;
	TSubclassOf<class AEHShooter> EHShooterClass;
	//TSubclassOf<class AEHArchor> EHArchorClass;

public:
	ALobbyGameMode();

	TArray<ALobbyPlayerController*> LobbyPlayerControllerArray;

	void TogglePlayerReady(APlayerController* Player);
	void UpdatePlayerNameListAndReadyState();
	void UpdatePlayerReadyState();
	void AddPlayerInfo(ALobbyPlayerController* NewLobbyPlayerController);
	void RemovePlayerInfo(const ALobbyPlayerController* ExitingLobbyPlayerController);
	bool PressGameStartButton();

	void SendChatMessage(const FText& Text);

	void UpdateCharacter(ALobbyPlayerController* LobbyPlayerController, EClassType ClassType);
	int FindLobbyPlayerSpot(ALobbyPlayerController* NewLobbyPlayerController);
};
