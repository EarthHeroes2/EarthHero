// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndGameMode.generated.h"

class FSharedContentHandle;
/**
 * 
 */
UCLASS()
class EARTHHERO_API AEndGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AEndGameMode();

	void SaveClientData(const FUniqueNetId& UserId, const FString& FileName, TArray<uint8>& Contents);
	void OnWriteSharedFileComplete(bool bWasSuccessful, const FUniqueNetId& UserId, const FString& FileName, const TSharedRef<FSharedContentHandle>& SharedHandle);
	void LoadCloudData(const FString& UserId, const FString& FileName);
	void OnReadSharedFileComplete(bool bWasSuccessful, const FSharedContentHandle& SharedContentHandle);
};
