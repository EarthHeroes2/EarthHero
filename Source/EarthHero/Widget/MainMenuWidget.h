// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MainMenuWidget.generated.h"


class UCheckBox;
class UButton;
class UTextBlock;
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
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	
	
	TSubclassOf<UUserWidget> OptionsWidgetClass;
	TSubclassOf<UUserWidget> LobbyRowWidgetClass;

	TArray<FString> LobbyIdList;
	TArray<ULobbyRowWidget*> LobbyRowList;
	
	TArray<UButton*> ButtonArray; //주요 버튼들 저장

	const FName JoinSessionName = "SessionName";
	FString LeaveSessionReason, FindSessionReason;
	FString ReceivedLobbyPort;

	FString ConnectString;
	FOnlineSessionSearchResult* SessionToJoin;

	FOnlineSessionSearchResult SelectedLobbyInfo;
	
	UUserWidget* OptionsWidget;

	
	UPROPERTY(meta = (BindWidget))
	UButton* Play_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Join_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Options_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Btn;

	UPROPERTY(meta = (BindWidget))
	UBorder* LobbySetting_Bd;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* LobbyName_Etb;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* Private_Cb;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordSetting_Etb;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateLobbyOK_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateLobbyCancel_Btn;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* Password_Bd;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Password_Etb;
	UPROPERTY(meta = (BindWidget))
	UButton* PasswordOK_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PasswordCancel_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* LobbyList_Btn;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* LobbyList_Scr;
	UPROPERTY(meta = (BindWidget))
	UBorder* LobbyList_Bd;
	UPROPERTY(meta = (BindWidget))
	UButton* FindLobby_Btn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Lev_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Exp_Tb;
	
	
	UFUNCTION()
	void Play_BtnClicked();
	UFUNCTION()
	void Join_BtnClicked();
	UFUNCTION()
	void OptionsBtnClicked();
	UFUNCTION()
	void Exit_BtnClicked();
	
	UFUNCTION()
	void PrivateCbChanged(bool bChecked);
	
	UFUNCTION()
	void CreateLobbyOKBtnClicked();
	UFUNCTION()
	void CreateLobbyCancelBtnClicked();

	UFUNCTION()
	void PasswordOKBtnClicked();
	UFUNCTION()
	void PasswordCancelBtnClicked();

	UFUNCTION()
	void LobbyListBtnClicked();
	UFUNCTION()
	void FindLobbyBtnClicked();

	
	void CreateLobbyWait();

	void MenuTearDown();

	void SetButtonsEnabled(bool bEnabled);
	
	void UpdateLobbyList(TArray<FOnlineSessionSearchResult> FindLobbyList);
	
	void FindLobbys(FString Reason);
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);
	
	void JoinSession();
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void LeaveSession(FString Reason);
	void DestroySessionComplete(FName SessionName, bool bWasSuccessful);
	
	FDelegateHandle FindSessionsDelegateHandle;
	FDelegateHandle JoinSessionDelegateHandle;
	FDelegateHandle DestroySessionCompleteDelegatesHandle;

public:
	void ServerRowClicked(FOnlineSessionSearchResult LobbyInfo, bool bAdvertise);
private:
	void PrivateServerRowClicked();
	void JoinToSelectedServer();

};
