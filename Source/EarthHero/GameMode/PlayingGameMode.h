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

	virtual void BeginPlay() override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	//플레이어 나간 것은 게임 세션에서 처리?
	
	void InitLevelSetting();
	

	TArray<AEHPlayerController*> EHPlayerControllers;


public:
	int NumPlayersInSession;
	void PlayerLogOut(const AEHPlayerController* ExitingEHPlayerController);
};
