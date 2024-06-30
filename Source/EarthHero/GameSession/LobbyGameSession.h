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
	int MaxNumberOfPlayersInSession = 2; // 세션 최대 인원수 (임시로2)

protected:
	int NumberOfPlayersInSession = 0;

	FName SessionName = "SessionName"; //임시 //무슨 의미가 있을까

	FUniqueNetIdRepl HostPlayerId;
	APlayerController* NewPlayerPlayerController;

	const FString MainSessionMap = TEXT("/Game/Maps/StartupMap"); //임시
	const FString InGameMap = TEXT("/Game/Maps/TestMap"); //임시

	bool bSessionExists = false;

	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual bool ProcessAutoLogin();
	virtual void NotifyLogout(const APlayerController* ExitingPlayer);

public:
	void CreateSession(FString PortNumber);
	void ChangeAdvertiseState(bool bAdvertise);

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

	void ChangeMap(); //테스트용

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
