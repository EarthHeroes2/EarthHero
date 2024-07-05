// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendRowWidget.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

bool UFriendRowWidget::Initialize()
{
	Super::Initialize();
	
	return true;
}

void UFriendRowWidget::SetFriendInfo(TSharedRef<FOnlineFriend> Friend)
{
	FriendInfo = Friend;
	//FriendImage_Img //STEAM API 사용해야함
	FriendName_Tb->SetText(FText::FromString(FriendInfo->GetDisplayName()));
	FriendInvite_Btn->OnClicked.AddDynamic(this, &UFriendRowWidget::InviteClicked);
}

void UFriendRowWidget::InviteClicked()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Sessions = Subsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			
			FUniqueNetIdPtr FriendNetId = Subsystem->GetIdentityInterface()->CreateUniquePlayerId(FriendInfo->GetUserId()->ToString());
			if (FriendNetId.IsValid())
			{
				TArray<TSharedRef<const FUniqueNetId>> FriendsToInvite;
				FriendsToInvite.Add(FriendNetId.ToSharedRef());
				Sessions->SendSessionInviteToFriend(0, NAME_GameSession, *FriendNetId);
			}
		}
	}
}


