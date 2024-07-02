// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>
#include "LobbyWidget.generated.h"



enum class EClassType : uint8
{
	Warrior,
	Mechanic,
	Shooter,
	Archor,
	NumberOfClass
};

UCLASS()
class EARTHHERO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual bool Initialize();

private:
	int NumberOfPlayers;
	int bHost = false;

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

	TArray<UTextBlock*> PlayerTexts;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player1_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player2_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player3_Txt;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Player4_Txt;

	TArray<UButton*> KickButtons;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick1_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick2_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick3_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Kick4_Btn;



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
	class UScrollBox* Chat_Scr;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Chat_Etb;

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
	void WarriorClicked();
	UFUNCTION()
	void MechanicClicked();
	UFUNCTION()
	void ShooterClicked(); 
	UFUNCTION()
	void ArchorClicked();

	int NumberOfClass = 4;
	
	void ChangeSelectedButton(EClassType ClassType);

	UFUNCTION()
	void ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);




	
public:
	void UpdatePlayerNameList(const TArray<FString>& PlayerNameList);
	void UpdateReadyState(const TArray<bool>& PlayerReadyStateArray);
	void AddChatMessage(const FText& Text);
};
