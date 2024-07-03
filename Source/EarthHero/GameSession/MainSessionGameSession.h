// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "MainSessionGameSession.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMainSessionGameSession : public AGameSession
{
	GENERATED_BODY()

protected:
	int MaxNumberOfPlayersInSession = 1000; // ���� �ִ� �ο��� (�⺻ ������ 1000��, �ܴ̿� 4��)
	int NumberOfPlayersInSession = 0;

	FName SessionName = "SessionName"; //무슨 의미가

	bool bSessionExists = false;

	virtual void BeginPlay();

	virtual bool ProcessAutoLogin();
	virtual void NotifyLogout(const APlayerController* ExitingPlayer);

public:
	void CreateSession(FString PortNumber);

protected:
	void HandleCreateSessionCompleted(FName InSessionName, bool bWasSuccessful);


	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite = false);
	void HandleRegisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer);
	void HandleUnregisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);


	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle UnregisterPlayerDelegateHandle;
	
};
