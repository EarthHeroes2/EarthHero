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


//심레스 트래블 이후 새로운 컨트롤러 생김
void AGameOverGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	APlayerController* NewPlayerController = Cast<APlayerController>(NewController);

	if(NewPlayerController)
	{
		AGameOverPlayerController* GameOverPlayerController = Cast<AGameOverPlayerController>(NewController);
		if(GameOverPlayerController)
		{
			GameOverPlayerControllers.Add(GameOverPlayerController);
		}
	}
}

void AGameOverGameMode::Logout(AController* Exiting)
{
    AGameOverPlayerController* ExitingGameOverPlayerController = Cast<AGameOverPlayerController>(Exiting);
    if (ExitingGameOverPlayerController)
    {
    	UE_LOG(LogTemp, Log, TEXT("Remove exit player information..."));
	
    	int PlayerIndex = GameOverPlayerControllers.IndexOfByKey(ExitingGameOverPlayerController);
		
    	if (PlayerIndex != INDEX_NONE) GameOverPlayerControllers.RemoveAt(PlayerIndex);
    }

	Super::Logout(Exiting);
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