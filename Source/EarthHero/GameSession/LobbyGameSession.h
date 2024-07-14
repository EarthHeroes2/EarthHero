// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EHGameSession.h"
#include "LobbyGameSession.generated.h"

/**
 *
 */
UCLASS()
class EARTHHERO_API ALobbyGameSession : public AEHGameSession
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
protected:
	FUniqueNetIdRepl HostPlayerId;
	APlayerController* NewPlayerPlayerController;

	void HostAssignment(APlayerController* HostPlayer);
	void NewHostFind();
	
public:
	void ChangeAdvertiseState(bool bAdvertise);
	void ChangeLobbyName(FString LobbyName);

	bool GetAdvertiseState();
	
	void CreateSession(FString PortNumber);
protected:
	void HandleCreateSessionCompleted(FName InSessionName, bool bWasSuccessful);
	
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite = false) override;
	void HandleRegisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);

	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;
	void HandleUnregisterPlayerCompleted(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);
public:
	void StartSession();
protected:
	void HandleStartSessionCompleted(FName SessionName, bool bWasSuccessful);
	
	FDelegateHandle UnregisterPlayerDelegateHandle;
	FDelegateHandle CreateSessionDelegateHandle;
	FDelegateHandle RegisterPlayerDelegateHandle;
	FDelegateHandle StartSessionDelegateHandle;
};
