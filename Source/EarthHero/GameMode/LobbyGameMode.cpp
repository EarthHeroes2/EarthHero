// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <EarthHero/GameSession/LobbyGameSession.h>
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode<AGameModeBase>();
	if (IsValid(GameModeBase))
	{
		GameModeBase->bUseSeamlessTravel = true;
	}
}

void ALobbyGameMode::AddPlayerReadyState(APlayerController* NewPlayer)
{
	ALobbyPlayerController* LobbyNewPlayerController = Cast<ALobbyPlayerController>(NewPlayer);

	int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(LobbyNewPlayerController);

	if (PlayerIndex != INDEX_NONE)
	{
		LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
		PlayerNameArray.RemoveAt(PlayerIndex);
		PlayerReadyStateArray.RemoveAt(PlayerIndex);
	}

	LobbyPlayerControllerArray.Add(LobbyNewPlayerController);
	PlayerNameArray.Add(LobbyNewPlayerController->PlayerState->GetPlayerName());
	PlayerReadyStateArray.Add(false);

	UpdatePlayerNameyListAndReadyState();
}

void ALobbyGameMode::TogglePlayerReady(APlayerController* Player)
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Player);

	int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(LobbyPlayerController);

	if (PlayerIndex != INDEX_NONE)
	{
		if (PlayerReadyStateArray[PlayerIndex]) ReadyCount--;
		else ReadyCount++;

		PlayerReadyStateArray[PlayerIndex] = !PlayerReadyStateArray[PlayerIndex];

		UpdatePlayerReadyState();
	}
	else UE_LOG(LogTemp, Error, TEXT("%s is not valid for player ready state"), LobbyPlayerController);
}

//��� Ŭ���̾�Ʈ���� �÷��̾� �̸� ����Ʈ ���� �� UpdatePlayerReadyState() ȣ��
void ALobbyGameMode::UpdatePlayerNameyListAndReadyState()
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Send player name list to client. (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_UpdatePlayerNameList(PlayerNameArray);
		}
	}

	UpdatePlayerReadyState();
}

//��� Ŭ���̾�Ʈ���� �ٲ� ���� ���� �迭 ����
void ALobbyGameMode::UpdatePlayerReadyState()
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();
	//ALobbyPlayerController�� for���� �ȵǳ���...

	UE_LOG(LogTemp, Log, TEXT("Send Ready state to client. (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_UpdateReadyState(PlayerReadyStateArray);
		}
	}
}

bool ALobbyGameMode::PressGameStartButton()
{
	ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(GameSession);
	if (LobbyGameSession)
	{
		if (ReadyCount == (LobbyGameSession->MaxNumberOfPlayersInSession - 1))
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby Start!"));
			LobbyGameSession->StartSession();
			return true;
		}
	}
	return false;
}