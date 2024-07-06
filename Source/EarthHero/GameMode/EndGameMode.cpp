// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameMode.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSharedCloudInterface.h"
#include "EarthHero/Player/EHPlayerState.h"


AEndGameMode::AEndGameMode()
{
	PlayerStateClass = AEHPlayerState::StaticClass();
}

class IOnlineSharedCloud;

void AEndGameMode::SaveClientData(const FUniqueNetId& UserId, const FString& FileName, TArray<uint8>& Contents)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSharedCloudPtr SharedCloud = Subsystem->GetSharedCloudInterface();
		if (SharedCloud.IsValid())
		{
			// 콜백 함수 등록
			SharedCloud->AddOnWriteSharedFileCompleteDelegate_Handle(
				FOnWriteSharedFileCompleteDelegate::CreateUObject(this, &AEndGameMode::OnWriteSharedFileComplete)
			);
			
			SharedCloud->WriteSharedFile(UserId, FileName, Contents);
		}
	}
}

void AEndGameMode::OnWriteSharedFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName, const TSharedRef<FSharedContentHandle>& SharedHandle)
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


void AEndGameMode::LoadCloudData(const FString& UserId, const FString& FileName)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSharedCloudPtr SharedCloud = Subsystem->GetSharedCloudInterface();
		if (SharedCloud.IsValid())
		{
			
			SharedCloud->AddOnReadSharedFileCompleteDelegate_Handle(
				FOnReadSharedFileCompleteDelegate::CreateUObject(this, &AEndGameMode::OnReadSharedFileComplete)
			);

			//FSharedContentHandle SharedHandle; ??????????
			//SharedCloud->ReadSharedFile(SharedHandle);
		}
	}
}

void AEndGameMode::OnReadSharedFileComplete(bool bWasSuccessful, const FSharedContentHandle& SharedContentHandle)
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