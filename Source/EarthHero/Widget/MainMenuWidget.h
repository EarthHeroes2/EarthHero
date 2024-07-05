// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuWidget.generated.h"


class UBorder;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPersonCPP/Maps/Lobby")));

protected:
	UFUNCTION()
	
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* Play_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* Join_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Btn;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* Private_Cb;

	UFUNCTION()
	void Play_BtnClicked();

	UFUNCTION()
	void Join_BtnClicked();

	UFUNCTION()
	void Exit_BtnClicked();

	void MenuTearDown();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };





	
	UPROPERTY(meta = (BindWidget))
	UButton* LobbyList_Btn;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LobbyList_Vb;

	UPROPERTY(meta = (BindWidget))
	UBorder* LobbyList_Bd;

	UFUNCTION()
	void LobbyListBtnClicked();
	void UpdateLobbyList();
};
