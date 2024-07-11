// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlayingGameState.generated.h"

class AEHPlayerController;
/**
 * 
 */
UCLASS()
class EARTHHERO_API APlayingGameState : public AGameStateBase
{
	GENERATED_BODY()

	AEHPlayerController* EHPlayerController;

public:
	virtual void BeginPlay() override;
	void UpdateHUDGameTimer(int GameTimer);
	void UpdateGameStateHealths(TArray<float> PlayerMaxHealths, TArray<float> PlayerCurrentHealths);
	void UpdateGameStateLevels(TArray<int> PlayerLevels);
	void UpdateGameStateExps(TArray<float> PlayerExps);

	UPROPERTY(ReplicatedUsing = OnRep_GameTimerSec)
	int GameTimerSec;
	UPROPERTY(Replicated)
	TArray<float> AllPlayerMaxHealths;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateHealths)
	TArray<float> AllPlayerCurrentHealths;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateLevels)
	TArray<int> AllPlayerLevels;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateExps)
	TArray<float> AllPlayerExps;

	UFUNCTION()
	void OnRep_GameTimerSec();
	UFUNCTION()
	void OnRep_GameStateHealths();
	UFUNCTION()
	void OnRep_GameStateLevels();
	UFUNCTION()
	void OnRep_GameStateExps();
};
