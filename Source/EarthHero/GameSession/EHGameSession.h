// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "EHGameSession.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AEHGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	const int MaxNumberOfPlayersInSession = 4;
	int GetNumPlayersInSession();

protected:
	int NumberOfPlayersInSession = 0;

	const FName SessionName = "NAME_GameSession"; 
	const FString InGameMap = TEXT("/Game/Maps/BattleRoyale_Map_a");
	
	virtual void BeginPlay() override;

	virtual bool ProcessAutoLogin() override;
	
	virtual void NotifyLogout(const APlayerController* ExitingPlayer) override; //일단 여기

public:
	virtual void UpdateNumberOfJoinedPlayers();

protected:
	void EndSession();
	void HandleEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	void DestroySession();
	void HandleDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	
	void HandleUpdateSessionCompleted(FName SessionName, bool bWasSuccessful);
	FString GetServerPort();

	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;
	FDelegateHandle UpdateSessionDelegateHandle;
};

