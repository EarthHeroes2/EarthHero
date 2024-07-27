// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/EHCharacter.h"
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
	void UpdateGameStateKillCount(const TArray<int> PlayerKillCount);
	void UpdateGameStateGivenDamage(const TArray<float> PlayerGivenDamage);
	void UpdateGameStateReceiveDamage(const TArray<float> PlayerReceiveDamage);
	void UpdateGameStateHeal(const TArray<float> PlayerHeal);
	void UpdateGameStateWorldMaps(const TArray<FVector2D> ActorLocations, const TArray<float> ActorRotations, const int32 PlayerNumbers);
	void SetGameStateForceField(const TArray<float> ExpansionDurations, const TArray<FVector2D> ForceFieldLocations);
	void UpatePlayerClassImage(const TArray<UTexture2D*> PlayerClassImages);

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
	UPROPERTY(ReplicatedUsing = OnRep_GameStateKillCount)
	TArray<int> AllPlayerKillCount;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateGivenDamage)
	TArray<float> AllPlayerGivenDamage;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateReceiveDamage)
	TArray<float> AllPlayerReceiveDamage;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateHeal)
	TArray<float> AllPlayerHeal;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateActorLocations)
	TArray<FVector2D> AllActorLocations;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateActorRotations)
	TArray<float> AllActorRotations;
	UPROPERTY(Replicated)
	int32 AllPlayerNumbers;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateExpansionDurations)
	TArray<float> AllExpansionDurations;
	UPROPERTY(ReplicatedUsing = OnRep_GameStateForceFieldLocations)
	TArray<FVector2D> AllForceFieldLocations;
	UPROPERTY(ReplicatedUsing = OnRep_GameStatePlayerClassImages)
	TArray<UTexture2D*> AllPlayerClassImages;

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
	UFUNCTION()
	void OnRep_GameStateKillCount() const;
	UFUNCTION()
	void OnRep_GameStateGivenDamage() const;
	UFUNCTION()
	void OnRep_GameStateReceiveDamage() const;
	UFUNCTION()
	void OnRep_GameStateHeal() const;
	UFUNCTION()
	void OnRep_GameStateActorLocations() const;
	UFUNCTION()
	void OnRep_GameStateActorRotations() const;
	UFUNCTION()
	void OnRep_GameStateExpansionDurations() const;
	UFUNCTION()
	void OnRep_GameStateForceFieldLocations() const;
	UFUNCTION()
	void  OnRep_GameStatePlayerClassImages() const;
};
