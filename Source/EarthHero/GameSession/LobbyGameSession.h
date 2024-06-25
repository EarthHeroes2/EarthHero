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

protected:
	int MaxNumberOfPlayersInSession = 2; // ���� �ִ� �ο��� (�ӽ÷�2)
	int NumberOfPlayersInSession = 0;

	FName SessionName = "SessionName"; //�ӽ� //���� �ǹ̰� ������

	FUniqueNetIdRepl HostPlayerId;

	const FString MainSessionMap = TEXT("/Game/Maps/StartupMap"); //�ӽ�
	const FString InGameMap = TEXT("/Game/Maps/ForceFieldTestMap"); //�ӽ�

	bool bSessionExists = false;

	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual bool ProcessAutoLogin();
	virtual void NotifyLogout(const APlayerController* ExitingPlayer);

public:
	void CreateSession(FString PortNumber);

protected:
	void HandleCreateSessionCompleted(FName InSessionName, bool bWasSuccessful);


	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite = false);
	void HandleRegisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	void HostAssignment(APlayerController* HostPlayer);


	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer);
	void HandleUnregisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);


	void StartSession();
	void HandleStartSessionCompleted(FName SessionName, bool bWasSuccessful);

	void ChangeMap(); //�׽�Ʈ��

	void EndSession();
	void HandleEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	void DestroySession();
	void HandleDestroySessionCompleted(FName SessionName, bool bWasSuccessful);


	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle UnregisterPlayerDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;
	FDelegateHandle EndSessionDelegateHandle;
	FDelegateHandle DestroySessionDelegateHandle;

};
