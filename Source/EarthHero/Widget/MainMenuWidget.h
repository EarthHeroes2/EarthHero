// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MainMenuWidget.generated.h"


class UEditableTextBox;
class UScrollBox;
class ULobbyRowWidget;
class UBorder;
class UVerticalBox;

class FOnlineSessionSearch;
class FOnlineSessionSearchResult;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	UMainMenuWidget(const FObjectInitializer &ObjectInitializer);
	TSubclassOf<UUserWidget> OptionsWidgetClass;
	TSubclassOf<UUserWidget> LobbyRowWidgetClass;

	UUserWidget* OptionsWidget;
	
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

	const FName JoinSessionName = "SessionName";

	UPROPERTY(meta = (BindWidget))
	class UButton* Play_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* Join_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* Options_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Btn;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* Private_Cb;

	UFUNCTION()
	void Play_BtnClicked();

	UFUNCTION()
	void Join_BtnClicked();
	
	UFUNCTION()
	void OptionsBtnClicked();

	UFUNCTION()
	void Exit_BtnClicked();

	UFUNCTION()
	void CreateLobbyOKBtnClicked();

	UFUNCTION()
	void CreateLobbyCancleBtnClicked();

	void MenuTearDown();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{ 4 };
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };



	UPROPERTY(meta = (BindWidget))
	UBorder* LobbySetting_Bd;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* LobbyName_Etb;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreateLobbyOK_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateLobbyCancle_Btn;


	
	UPROPERTY(meta = (BindWidget))
	UButton* LobbyList_Btn;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* LobbyList_Scr;

	UPROPERTY(meta = (BindWidget))
	UBorder* LobbyList_Bd;

	UPROPERTY(meta = (BindWidget))
	UButton* FindLobby_Btn;

	UFUNCTION()
	void LobbyListBtnClicked();
	UFUNCTION()
	void FindLobbyBtnClicked();
	void UpdateLobbyList(TArray<FOnlineSessionSearchResult> FindLobbyList);
	void SetButtonsEnabled(bool bEnabled);
	void FindLobbys(FString Reason);
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	void JoinSession();
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
public:
	void LeaveSession(FString Reason);
private:
	void DestroySessionComplete(FName SessionName, bool bWasSuccessful);
	

	TArray<FString> LobbyIdList;
	TArray<ULobbyRowWidget*> LobbyRowList;


	
	//주요 버튼들 저장
	TArray<UButton*> ButtonArray;



protected:
	FString LeaveSessionReason, FindSessionReason;

public:
	FString ConnectString;
	FOnlineSessionSearchResult* SessionToJoin;
	
protected:
	FDelegateHandle FindSessionsDelegateHandle;
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle DestroySessionCompleteDelegatesHandle;
	
	
};
