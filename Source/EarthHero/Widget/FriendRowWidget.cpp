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

	//너무빨리 버튼을 생성해주나?
	FriendInvite_Btn->OnClicked.AddDynamic(this, &UFriendRowWidget::InviteClicked);
	
	return true;
}

void UFriendRowWidget::UpdateFriendInfo(TSharedRef<FOnlineFriend> Friend, bool bOnline)
{
	FriendInfo = Friend;

	//친구 온라인 여부에 따라 위젯 활성화
	if(bOnline) this->SetIsEnabled(true);
	else this->SetIsEnabled(false);
	
	FriendName_Tb->SetText(FText::FromString(FriendInfo->GetDisplayName()));
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
				Sessions->SendSessionInviteToFriend(0, "MainSession", *FriendNetId);
			}
		}
	}
}


