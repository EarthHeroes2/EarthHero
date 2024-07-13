// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverGameMode.generated.h"

class AGameOverPlayerController;
class FSharedContentHandle;
/**
 * 
 */
UCLASS()
class EARTHHERO_API AGameOverGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AGameOverGameMode();

	void SaveClientData(const FUniqueNetId& UserId, const FString& FileName, TArray<uint8>& Contents);
	void OnWriteSharedFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName, const TSharedRef<FSharedContentHandle>& SharedHandle);
	void LoadCloudData(const FString& UserId, const FString& FileName);
	void OnReadSharedFileComplete(bool bWasSuccessful, const FSharedContentHandle& SharedContentHandle);


public:
	void SendChatMessage(const FText& Text);


	TArray<AGameOverPlayerController*> GameOverPlayerControllers;
};
