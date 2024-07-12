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
	void UpdateGameStateHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths);
	void UpdateGameStateLevels(const TArray<int>& PlayerLevels);
	void UpdateGameStateExps(const TArray<float>& PlayerExps);
	void UpdateGameStateClasses(const TArray<int>& PlayerClasses);
	void UpdateGameStateNames(const TArray<FString>& PlayerNames);

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
	UPROPERTY(ReplicatedUsing = OnRep_GameStateClasses)
	TArray<int> AllPlayerClasses;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateNames)
	TArray<FString> AllPlayerNames;
	

	UFUNCTION()
	void OnRep_GameTimerSec() const;
	UFUNCTION()
	void OnRep_GameStateHealths() const;
	UFUNCTION()
	void OnRep_GameStateLevels() const;
	UFUNCTION()
	void OnRep_GameStateExps() const;
	UFUNCTION()
	void OnRep_GameStateClasses() const;
	UFUNCTION()
	void OnRep_GameStateNames() const;
};
