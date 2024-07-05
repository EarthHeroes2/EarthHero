// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "LobbyGameSession.generated.h"

/**
 *
 */
UCLASS()
class EARTHHERO_API ALobbyGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	int MaxNumberOfPlayersInSession = 4;

protected:
	int NumberOfPlayersInSession = 0;

	FName SessionName = "SessionName"; //무슨의미일까

	FUniqueNetIdRepl HostPlayerId;
	APlayerController* NewPlayerPlayerController;

	const FString MainSessionMap = TEXT("/Game/Maps/StartupMap"); //�ӽ�
	const FString InGameMap = TEXT("/Game/Maps/TestMap"); //�ӽ�

	bool bSessionExists = false;

	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual bool ProcessAutoLogin();
	virtual void NotifyLogout(const APlayerController* ExitingPlayer);

public:
	void CreateSession(FString PortNumber);
	void ChangeAdvertiseState(bool bAdvertise);
	void UpdateNumberOfJoinedPlayers();

protected:
	void HandleCreateSessionCompleted(FName InSessionName, bool bWasSuccessful);


	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite = false);
	void HandleRegisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	void HostAssignment(APlayerController* HostPlayer);


	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer);
	void HandleUnregisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

public:
	void StartSession();
protected:
	void HandleStartSessionCompleted(FName SessionName, bool bWasSuccessful);

	void ChangeMap(); //�׽�Ʈ��

	void EndSession();
	void HandleEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	void DestroySession();
	void HandleDestroySessionCompleted(FName SessionName, bool bWasSuccessful);


	void HandleUpdateSessionCompleted(FName SessionName, bool bWasSuccessful);

	void NewHostFind();


	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle UnregisterPlayerDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;
	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;

	FDelegateHandle UpdateSessionDelegateHandle;
};
