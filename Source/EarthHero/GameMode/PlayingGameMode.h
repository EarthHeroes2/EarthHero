#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayingGameMode.generated.h"

class AEHPlayerController;

UCLASS()
class EARTHHERO_API APlayingGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	void InitLevelSetting();
	void GameTimerCount();

	TArray<AEHPlayerController*> EHPlayerControllers;

	int NumPlayerControllerReady = 0;
	int NumDeadPlayers = 0;
	const FString GameOverMap = TEXT("/Game/Maps/GameOverMap");

public:
	int NumPlayersInSession;
	int GameTimer = 0;

	void PlayerLogOut(const AEHPlayerController* ExitingEHPlayerController);
	void SendChatMessage(const FText& Text);
	void AddPlayerDead();
	void CheckAllPlayerDead();
	void BossDead();
	void UpdateGameStateHealths();
	void UpdateGameStateLevels();
	void UpdateGameStateExps();
	void UpdateGameStateNames();
	void UpdateGameStateClasses();
	void UpdateGameStateKillCount();
	void UpdateGameStateDamage();
	void UpdateGameStateReceiveDamage();
	void UpdateGameStateHeal();

	void PlayerControllerReady();
	void EnableAllInput();

	void SpawnForceFields();
	void SpawnForceFieldAtLocation(FVector Location, float ExpansionDuration);
    
	bool IsValidForceFieldDistance(const TArray<FVector>& Locations, float MinDistance);
	void GenerateRandomDurations(int Count, float Min, float Max, TArray<float>& OutDurations);
};
