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
	int MaxNumberOfPlayersInSession = 4;

protected:
	int NumberOfPlayersInSession = 0;

	FName SessionName = "NAME_GameSession"; 
	
	const FString MainSessionMap = TEXT("/Game/Maps/StartupMap");
	const FString InGameMap = TEXT("/Game/Maps/TestMap");

	bool bSessionExists = false;

	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual bool ProcessAutoLogin();
	virtual void NotifyLogout(const APlayerController* ExitingPlayer);

public:
	void UpdateNumberOfJoinedPlayers();

protected:
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer);
	void HandleUnregisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);
protected:
	void EndSession();
	void HandleEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	void DestroySession();
	void HandleDestroySessionCompleted(FName SessionName, bool bWasSuccessful);


	void HandleUpdateSessionCompleted(FName SessionName, bool bWasSuccessful);

	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle UnregisterPlayerDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;
	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;

	FDelegateHandle UpdateSessionDelegateHandle;
};

