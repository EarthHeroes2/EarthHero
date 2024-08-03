#pragma once

#include "CoreMinimal.h"
#include "EarthHero/GameState/PlayingGameState.h"
#include "GameFramework/GameModeBase.h"
#include "PlayingGameMode.generated.h"

class AEffectBase;
class AEHShooter;
class AEHWarrior;
class AEHPlayerController;

UCLASS()
class EARTHHERO_API APlayingGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;

	void InitLevelSetting();
	void GameTimerCount();

	TArray<AEHPlayerController*> EHPlayerControllers;
	TArray<bool> bPlayerAlives;

	int NumPlayerControllerReady = 0;
	int NumDeadPlayers = 0;
	const FString GameOverMap = TEXT("/Game/Maps/GameOverMap");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "ForceField")
	TSubclassOf<class AForceField> ForceFieldActor;



	TArray<TSubclassOf<ACharacter>> CharacterClasses;
	TSubclassOf<AEHWarrior> EHWarriorClass;
	//TSubclassOf<class AEHMechanic> EHMechanicClass;
	TSubclassOf<AEHShooter> EHShooterClass;
	//TSubclassOf<class AEHArchor> EHArchorClass;

protected:
	virtual void Tick(float DeltaSeconds) override;
	void UpdateWorldMapInfo();

	UPROPERTY()
	TArray<AActor*> Players;
	
public:
	APlayingGameMode();
	int NumPlayersInSession;
	int GameTimer = 0;

	//DEBUG_MODE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUG MODE")
	bool IsDebugMode = false;

	void PlayerLogOut(const AEHPlayerController* ExitingEHPlayerController);
	void SendChatMessage(const FText& Text);
	void AddPlayerDead(AEHPlayerController* DeadEHPlayerController, FVector DeadLocation);
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
	void UpdatePlayerClassImage();
	void UpdatePlayerStateImage(AActor *TargetActor, TSubclassOf<AEffectBase> EffectType);
	int32 FindControllerForTargetActor(AActor* TargetActor);

	void Rebirth(AEHPlayerController* EHPlayerController);

	void PlayerControllerReady();
	void GameStart();
	
	void InitForceFields();
	void SpawnForceFields();
	void SpawnForceFieldAtLocation(FVector2D Location, float ExpansionDuration);
    
	bool IsValidForceFieldDistance(const TArray<FVector2D>& Locations, float MinDistance);
	void GenerateRandomDurations(int Count, float Min, float Max, TArray<float>& OutDurations);

	bool GetPlayerLocation(int PlayerNumber, FVector& PlayerLocation);
	
private:
	TArray<float> ExpansionDurations;
	TArray<FVector2D> ForceFieldLocations;

	bool DebugMode;
};
