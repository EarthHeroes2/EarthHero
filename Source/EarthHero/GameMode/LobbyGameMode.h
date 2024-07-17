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

	ALobbyGameMode();
	
	virtual void BeginPlay() override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	
	int ReadyCount = 0;

	//플레이어 정보들 관리
	TArray<FVector> SpawnLocations;
	TArray<AController*> ControllerArray;
	TArray<ALobbyPlayerController*> LobbyPlayerControllerArray;
	TArray<FString> PlayerNameArray;
	TArray<bool> PlayerReadyStateArray;
	TArray<EClassType> PlayerClassArray;
	TArray<int> PlayerSpotArray;
	TArray<bool> bSpotUsedArray;

	int CallCount = 0;
	

	TArray<TSubclassOf<ACharacter>> CharacterClasses;
	//TSubclassOf<class AEHWarrior> EHWarriorClass;
	//TSubclassOf<class AEHMechanic> EHMechanicClass;
	TSubclassOf<class AEHShooter> EHShooterClass;
	//TSubclassOf<class AEHArchor> EHArchorClass;

	
	void UpdatePlayerNameListAndReadyState();
	void UpdatePlayerReadyState();
	
	int FindLobbyPlayerSpot();
	
public:
	bool PressGameStartButton();
	void TogglePlayerReady(APlayerController* Player);

	void JoinedPlayerInitSetup(ALobbyPlayerController* NewLobbyPlayerController);
	
	void RemovePlayerInfo(const ALobbyPlayerController* ExitingLobbyPlayerController);

	void SendChatMessage(const FText& Text);
	
	void UpdateDifficulty(int Difficulty);

	void UpdateCharacter(ALobbyPlayerController* LobbyPlayerController, EClassType ClassType);
	
	ALobbyPlayerController* GetFirstLobbyPlayerController();

	void Kick(int PlayerNumber);
};
