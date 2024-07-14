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

protected:
	UFUNCTION()
	
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	

private:

	const FName JoinSessionName = "SessionName";
	FString ReceivedLobbyPort;

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
	void LobbyNameEtbChanged(const FText& Text);
	UFUNCTION()
	void PasswordEtbChanged(const FText& Text);

	UFUNCTION()
	void CreateLobbyOKBtnClicked();

	UFUNCTION()
	void CreateLobbyCancleBtnClicked();

	UFUNCTION()
	void PasswordOKBtnClicked();
	UFUNCTION()
	void PasswordCancleBtnClicked();

	
	void CreateLobbyWait();

	void MenuTearDown();
	
	

	UPROPERTY(meta = (BindWidget))
	UBorder* LobbySetting_Bd;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* LobbyName_Etb;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordSetting_Etb;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreateLobbyOK_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateLobbyCancle_Btn;

	
	
	UPROPERTY(meta = (BindWidget))
	UBorder* Password_Bd;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Password_Etb;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PasswordOK_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* PasswordCancle_Btn;


	
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
	void PrivateServerRowClicked();
	void ServerRowClicked();

private:
	void DestroySessionComplete(FName SessionName, bool bWasSuccessful);
	

	TArray<FString> LobbyIdList;
	TArray<ULobbyRowWidget*> LobbyRowList;


	
	//주요 버튼들 저장
	TArray<UButton*> ButtonArray;
public:
	FOnlineSessionSearchResult SelectedLobbyInfo;


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
