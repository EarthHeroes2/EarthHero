// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FriendRowWidget.generated.h"

class FOnlineFriend;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UFriendRowWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;

public:
	void UpdateFriendInfo(TSharedRef<FOnlineFriend> Friend, bool bOnline);

private:
	TSharedPtr<FOnlineFriend> FriendInfo;

	UFUNCTION()
	void InviteClicked();
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FriendName_Tb;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* FriendInvite_Btn;

	
};
