// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverGameMode.h"
#include "EarthHero/GameSession/EHGameSession.h"
#include "EarthHero/GameSession/GameOverGameSession.h"
#include "EarthHero/PlayerController/GameOverPlayerController.h"
#include "EarthHero/PlayerState/GameOverPlayerState.h"


AGameOverGameMode::AGameOverGameMode()
{
	PlayerStateClass = AGameOverPlayerState::StaticClass();
	PlayerControllerClass = AGameOverPlayerController::StaticClass();
	GameSessionClass = AGameOverGameSession::StaticClass();
}

void AGameOverGameMode::BeginPlay()
{
	Super::BeginPlay();
	bUseSeamlessTravel = true;
}


//심레스 트래블 이후 새로운 컨트롤러 생김
void AGameOverGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	APlayerController* NewPlayerController = Cast<APlayerController>(NewController);
	if(NewPlayerController)
	{
		AGameOverPlayerController* NewGameOverPlayerController = Cast<AGameOverPlayerController>(NewController);
		if(NewGameOverPlayerController)
		{
			AGameOverGameSession* GameOverGameSession = Cast<AGameOverGameSession>(GameSession);
			if (GameOverGameSession)
			{
				UE_LOG(LogTemp, Warning, TEXT("444444444444444444444444"));
				GameOverPlayerControllers.Add(NewGameOverPlayerController);
				GameOverGameSession->SeamlessTravelSuccessCount();
			}
		}
	}
}

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