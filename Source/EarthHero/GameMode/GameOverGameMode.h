// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverGameMode.generated.h"

class AGameOverPlayerController;
class FSharedContentHandle;

UCLASS()
class EARTHHERO_API AGameOverGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AGameOverGameMode();

private:
	TSharedPtr<FSharedContentHandle> TestSharedHandle;

	
public:
	void SendChatMessage(const FText& Text);


	TArray<AGameOverPlayerController*> GameOverPlayerControllers;
};
