// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayingGameMode.generated.h"

class AEHPlayerController;
/**
 * 
 */
UCLASS()
class EARTHHERO_API APlayingGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	TArray<AEHPlayerController*> AllConnectedPlayerControllers;
};
