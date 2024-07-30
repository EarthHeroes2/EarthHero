// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>
#include <steam/steamclientpublic.h>

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "EarthHero/Enum/Enums.h"
#include "LobbyWidget.generated.h"

class UImage;
class UFriendRowWidget;

UCLASS()
class EARTHHERO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	ULobbyWidget(const FObjectInitializer &ObjectInitializer);
	TSubclassOf<class UUserWidget> FriendRowWidgetClass;
	
	virtual bool Initialize() override;
	void ReadFriendsList();

public:
	void HostAssignment(bool bHostAssignment, bool bAdvertise, int Difficulty);

private:
	int MaxNumberOfPlayers = 4; //원래 로비 게임 세션에도 있지만... 어차피 4명 고정게임이라 
	int NumberOfPlayers;
	int bHost = false;
	EClassType SelectClass = Warrior;
	int SelectDifficulty = 1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Ready_Btn;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyButton_Tb;

	UPROPERTY(meta = (BindWidget))
	UButton* Player1_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Player2_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Player3_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Player4_Btn;

	TArray<UTextBlock*> PlayerNameTexts;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player3_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player4_Txt;
	
	TArray<UTextBlock*> PlayerLevelTexts;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1Level_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2Level_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player3Level_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player4Level_Txt;

	TArray<UButton*> KickButtons;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick1_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick2_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick3_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick4_Btn;

	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Class_Vb;
	
	TArray<UButton*> ClassBtns;
	UPROPERTY(meta = (BindWidget))
	UButton* Warrior_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Mechanic_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Shooter_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Archor_Btn;

	UPROPERTY(meta = (BindWidget))
	UImage* RoomSetting_Img;

	TArray<UButton*> DifficultyBtns;
	UPROPERTY(meta = (BindWidget))
	UButton* Difficulty1_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Difficulty2_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Difficulty3_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Difficulty4_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Difficulty5_Btn;


	UPROPERTY(meta = (BindWidget))
	UScrollBox* Chat_Scr;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Chat_Etb;


	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LobbySettings_Vb;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Private_Hb;
public:
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* Private_Cb;
private:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* PasswordSetting_Hb;
public:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Password_Etb;
private:
	UPROPERTY(meta = (BindWidget))
	UButton* PasswordUpdate_Btn;

	
	
	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Btn;

	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* Friend_Scr;

	UFUNCTION()
	void ReadyClicked();

	UFUNCTION()
	void Player1Hovered();
	UFUNCTION()
	void Player2Hovered();
	UFUNCTION()
	void Player3Hovered();
	UFUNCTION()
	void Player4Hovered();

	UFUNCTION()
	void Player1Unhovered();
	UFUNCTION()
	void Player2Unhovered();
	UFUNCTION()
	void Player3Unhovered();
	UFUNCTION()
	void Player4Unhovered();

	void VisibleKickButton(int PlayerNumber);
	void InvisibleKickButton(int PlayerNumber);

	UFUNCTION()
	void Kick1Clicked();
	UFUNCTION()
	void Kick2Clicked();
	UFUNCTION()
	void Kick3Clicked();
	UFUNCTION()
	void Kick4Clicked();

	void PlayerKick(int PlayerNumber);

	UFUNCTION()
	void Difficulty1BtnClicked();
	UFUNCTION()
	void Difficulty2BtnClicked();
	UFUNCTION()
	void Difficulty3BtnClicked();
	UFUNCTION()
	void Difficulty4BtnClicked();
	UFUNCTION()
	void Difficulty5BtnClicked();
	
	void SetDifficulty(int Difficulty);


	UFUNCTION()
	void WarriorClicked();
	UFUNCTION()
	void MechanicClicked();
	UFUNCTION()
	void ShooterClicked(); 
	UFUNCTION()
	void ArchorClicked();


	UFUNCTION()
	void ChangePrivateState(bool bChecked);
	
	UFUNCTION()
	void ExitClicked();

	int NumberOfClass = 4;
public:
	void ChangeSelectedButton(EClassType ClassType);
protected:
	void SetPlayerCharacter(EClassType ClassType);
	UFUNCTION()
	void ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void PasswordUpdateBtnClicked();
	
public:
	void UpdatePlayerNameList(const TArray<FString>& PlayerNameList, const TArray<int> PlayerLevelList);
	void UpdateReadyState(const TArray<bool>& PlayerReadyStateArray);
	void UpdateDifficulty(const int Difficulty);
	
	void AddChatMessage(const FText& Text);


	FTimerHandle ReadFriendsListTimerHandle;

	bool bPasswordSetting = false;

protected:
	void ReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	
	TArray<CSteamID> FriendSteamIds;
	TArray<UFriendRowWidget*> FriendRowWidgets;

	
	
	

	
};
