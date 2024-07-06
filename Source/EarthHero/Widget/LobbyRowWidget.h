// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyRowWidget.generated.h"


class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class EARTHHERO_API ULobbyRowWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize();

public:
	void UpdateLobbyInfo(FOnlineSessionSearchResult Lobby);

private:

	UFUNCTION()
	void JoinClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* LobbyJoin_Btn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LobbyName_Tb;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCount_Tb;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ping_Tb;
	
};
