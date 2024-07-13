// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverGameMode.h"

#include "OnlineSubsystem.h"
#include "EarthHero/GameSession/EHGameSession.h"
#include "Interfaces/OnlineSharedCloudInterface.h"
#include "EarthHero/PlayerController/GameOverPlayerController.h"
#include "EarthHero/PlayerState/GameOverPlayerState.h"


AGameOverGameMode::AGameOverGameMode()
{
	PlayerStateClass = AGameOverPlayerState::StaticClass();
	PlayerControllerClass = AGameOverPlayerController::StaticClass();
	GameSessionClass = AEHGameSession::StaticClass();
}

class IOnlineSharedCloud;

void AGameOverGameMode::SaveClientData(const FUniqueNetId& UserId, const FString& FileName, TArray<uint8>& Contents)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSharedCloudPtr SharedCloud = Subsystem->GetSharedCloudInterface();
		if (SharedCloud.IsValid())
		{
			// 콜백 함수 등록
			SharedCloud->AddOnWriteSharedFileCompleteDelegate_Handle(
				FOnWriteSharedFileCompleteDelegate::CreateUObject(this, &AGameOverGameMode::OnWriteSharedFileComplete)
			);
			
			SharedCloud->WriteSharedFile(UserId, FileName, Contents);
		}
	}
}

void AGameOverGameMode::OnWriteSharedFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName, const TSharedRef<FSharedContentHandle>& SharedHandle)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully saved file %s for user %s"), *FileName, *UserId.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save file %s for user %s"), *FileName, *UserId.ToString());
	}
}


void AGameOverGameMode::LoadCloudData(const FString& UserId, const FString& FileName)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSharedCloudPtr SharedCloud = Subsystem->GetSharedCloudInterface();
		if (SharedCloud.IsValid())
		{
			
			SharedCloud->AddOnReadSharedFileCompleteDelegate_Handle(
				FOnReadSharedFileCompleteDelegate::CreateUObject(this, &AGameOverGameMode::OnReadSharedFileComplete)
			);
			//SharedCloud->ReadSharedFile()

			//FSharedContentHandle SharedHandle; ??????????
			//SharedCloud->ReadSharedFile(SharedHandle);
		}
	}
}

void AGameOverGameMode::OnReadSharedFileComplete(bool bWasSuccessful, const FSharedContentHandle& SharedContentHandle)
{
	if (bWasSuccessful)
	{
		//어떻게 해야하나...
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to read shared file"));
	}
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