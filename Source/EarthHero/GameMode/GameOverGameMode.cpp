// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverGameMode.h"
#include "EarthHero/GameSession/EHGameSession.h"
#include "EarthHero/PlayerController/GameOverPlayerController.h"
#include "EarthHero/PlayerState/GameOverPlayerState.h"


AGameOverGameMode::AGameOverGameMode()
{
	PlayerStateClass = AGameOverPlayerState::StaticClass();
	PlayerControllerClass = AGameOverPlayerController::StaticClass();
	GameSessionClass = AEHGameSession::StaticClass();
}


//모든 플레이어 정보를 서버에 저장...





void AGameOverGameMode::SendChatMessage(const FText& Text)
{
	int32 NumberOfPlayers = GameOverPlayerControllers.Num();

	UE_LOG(LogTemp, Log, TEXT("Send a message to clients"));

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (GameOverPlayerControllers[i])
		{
			GameOverPlayerControllers[i]->Client_SendChatMessage(Text);
		}
	}
}