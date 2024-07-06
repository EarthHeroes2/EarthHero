// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Stat/Structure/StatStructure.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EHGameInstance.generated.h"

//Need to forward declare classes used 
class FOnlineSessionSearch;
class FOnlineSessionSearchResult;

UCLASS()
class EARTHHERO_API UEHGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

public:
	FStatStructure* GetStatStructure(FName HeroName) const;
	
protected:
	UEHGameInstance();

private:
	class UDataTable* CharacterStatDataTable = nullptr;

public:
	bool IsCheckedPrivate = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* MainSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* MasterVolumeSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* BackgroundVolumeSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* SFXVolumeSoundClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float BackgroundVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SFXVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ResolutionWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ResolutionHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ScreenMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 MaxFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bVSyncEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 OverallQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 AntiAliasing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 PostProcessing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MouseSensitivity;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings();
protected:
	FName JoinedSessionName;
	FString LeaveSessionReason, FindSessionReason;
public:
	void FindSessions(FString Reason);
protected:
	// Callback function. This function will run when the session is found.
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	FDelegateHandle FindSessionsDelegateHandle;

	// This is the connection string for the client to connect to the dedicated server.
	FString ConnectString;

	// This is used to store the session to join information from the search. You could pass it as a paramter to JoinSession() instead. 
	FOnlineSessionSearchResult* SessionToJoin;

	// Function to join the EOS session. 
	void JoinSession();

	// Callback function. This function will run when the session is joined. 
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Delegate to bind callback event for join session.
	FDelegateHandle JoinSessionDelegateHandle;

public:
	void LeaveSession(FString Reason);

protected:
	void DestroySessionComplete(FName SessionName, bool bWasSuccessful);
	FDelegateHandle DestroySessionCompleteDelegatesHandle;
};
