// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>

#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "EarthHero/EHGameInstance.h"

/*
ULobbyWidget::ULobbyWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//친구 초대 row 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> FriendRowWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Friend_Row.WBP_Friend_Row_C'"));
	if (FriendRowWidgetAsset.Succeeded())
	{
		FriendRowWidgetClass = FriendRowWidgetAsset.Class;
	}
}*/

bool ULobbyWidget::Initialize()
{
	Super::Initialize();

	PlayerTexts.Add(Player1_Txt);
	PlayerTexts.Add(Player2_Txt);
	PlayerTexts.Add(Player3_Txt);
	PlayerTexts.Add(Player4_Txt);

	ClassBtns.Add(Warrior_Btn);
	ClassBtns.Add(Mechanic_Btn);
	ClassBtns.Add(Shooter_Btn);
	ClassBtns.Add(Archor_Btn);

	Warrior_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::WarriorClicked);
	Mechanic_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::MechanicClicked);
	Shooter_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ShooterClicked);
	Archor_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ArchorClicked);

	Chat_Etb->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ChatTextCommitted);

	Private_Cb->OnCheckStateChanged.AddDynamic(this, &ULobbyWidget::ChangePrivateState);

	Exit_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ExitClicked);

	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	if (EHGameInstance)
	{
		if (EHGameInstance->IsCheckedPrivate) Private_Cb->SetCheckedState(ECheckBoxState::Checked);
		else Private_Cb->SetCheckedState(ECheckBoxState::Unchecked);
	}

	/*
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineFriendsPtr Friends = Subsystem->GetFriendsInterface();
		if (Friends)
		{
			Friends->ReadFriendsList(
				0,
				TEXT(""),
				FOnReadFriendsListComplete::CreateUObject(this, &ULobbyWidget::ReadFriendsListCompleted)
			);
		}
	}*/
	return true;
}

/*
void ULobbyWidget::ReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	if (bWasSuccessful)
	{
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem)
		{
			IOnlineFriendsPtr Friends = Subsystem->GetFriendsInterface();
			if (Friends)
			{
				if (FriendRowWidgetClass)
				{
					//위젯 생성
					UUserWidget* FriendRowWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), FriendRowWidgetClass));

					//친구 리스트 얻고
					TArray<TSharedRef<FOnlineFriend>> FriendList;
					Friends->GetFriendsList(LocalUserNum, ListName, FriendList);

					//하나씩 추가
					for (TSharedRef<FOnlineFriend> Friend : FriendList)
					{
						if (FriendRowWidget)
						{
							UTextBlock* FriendName_Tb = Cast<UTextBlock>(FriendRowWidget->GetWidgetFromName(TEXT("FriendName_Tb")));
							if (FriendName_Tb)
							{
								FriendName_Tb->SetText(FText::FromString(Friend->GetDisplayName()));
							}

							
                            UImage* FriendImage_Img = Cast<UImage>(FriendRowWidget->GetWidgetFromName(TEXT("FriendImage_Img")));
                            if (FriendImage_Img)
                            {
                                //STEAM API 사용해야함
                            }
                            
							
                            UButton* FriendInvite_Btn = Cast<UButton>(FriendRowWidget->GetWidgetFromName(TEXT("FriendInvite_Btn")));
                            if (FriendInvite_Btn)
                            {
                            }

							Friend_Scr->AddChild(FriendRowWidget);
						}
					}
				}
			}
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("Failed to read Firends list"));
}*/


void ULobbyWidget::HostAssignment(bool bHostAssignment)
{
	bHost = bHostAssignment;
	
	if (bHost)
		ReadyButton_Tb->SetText(FText::FromString("Game Start"));
	else
		ReadyButton_Tb->SetText(FText::FromString("Game Ready"));
	
	Ready_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ReadyClicked);
	
	if (bHost)
	{
		Player1_Btn->OnHovered.AddDynamic(this, &ULobbyWidget::Player1Hovered);
		Player2_Btn->OnHovered.AddDynamic(this, &ULobbyWidget::Player2Hovered);
		Player3_Btn->OnHovered.AddDynamic(this, &ULobbyWidget::Player3Hovered);
		Player4_Btn->OnHovered.AddDynamic(this, &ULobbyWidget::Player4Hovered);
		Player1_Btn->OnUnhovered.AddDynamic(this, &ULobbyWidget::Player1Unhovered);
		Player2_Btn->OnUnhovered.AddDynamic(this, &ULobbyWidget::Player2Unhovered);
		Player3_Btn->OnUnhovered.AddDynamic(this, &ULobbyWidget::Player3Unhovered);
		Player4_Btn->OnUnhovered.AddDynamic(this, &ULobbyWidget::Player4Unhovered);

		KickButtons.Add(Kick1_Btn);
		KickButtons.Add(Kick2_Btn);
		KickButtons.Add(Kick3_Btn);
		KickButtons.Add(Kick4_Btn);

		Kick1_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::Kick1Clicked);
		Kick2_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::Kick2Clicked);
		Kick3_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::Kick3Clicked);
		Kick4_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::Kick4Clicked);

		Private_Hb->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget::ReadyClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Ready Button Clicked"));

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);

		if (LobbyPlayerController)
		{
			LobbyPlayerController->Server_ClientReady();
		}
	}
}

void ULobbyWidget::Player1Hovered()
{
	VisibleKickButton(1);
}
void ULobbyWidget::Player2Hovered()
{
	VisibleKickButton(2);
}
void ULobbyWidget::Player3Hovered()
{
	VisibleKickButton(3);
}
void ULobbyWidget::Player4Hovered()
{
	VisibleKickButton(4);
}
void ULobbyWidget::VisibleKickButton(int PlayerNumber)
{
	if(NumberOfPlayers >= PlayerNumber)
		KickButtons[PlayerNumber - 1]->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidget::Player1Unhovered()
{
	InvisibleKickButton(1);
}
void ULobbyWidget::Player2Unhovered()
{
	InvisibleKickButton(2);
}
void ULobbyWidget::Player3Unhovered()
{
	InvisibleKickButton(3);
}
void ULobbyWidget::Player4Unhovered()
{
	InvisibleKickButton(4);
}
void ULobbyWidget::InvisibleKickButton(int PlayerNumber)
{
	if (NumberOfPlayers >= PlayerNumber)
		KickButtons[PlayerNumber - 1]->SetVisibility(ESlateVisibility::Collapsed);
}

void ULobbyWidget::Kick1Clicked()
{
	PlayerKick(1);
}
void ULobbyWidget::Kick2Clicked()
{
	PlayerKick(2);
}
void ULobbyWidget::Kick3Clicked()
{
	PlayerKick(3);
}
void ULobbyWidget::Kick4Clicked()
{
	PlayerKick(4);
}
void ULobbyWidget::PlayerKick(int PlayerNumber)
{
	UE_LOG(LogTemp, Log, TEXT("Player %d kick request"), PlayerNumber-1);

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		if (LobbyPlayerController)
		{
			LobbyPlayerController->Server_PlayerKick(PlayerNumber - 1);
		}
	}
}


void ULobbyWidget::WarriorClicked()
{
	ChangeSelectedButton(Warrior);
}
void ULobbyWidget::MechanicClicked()
{
	ChangeSelectedButton(Mechanic);
}
void ULobbyWidget::ShooterClicked()
{
	ChangeSelectedButton(Shooter);
}
void ULobbyWidget::ArchorClicked()
{
	ChangeSelectedButton(Archor);
}

void ULobbyWidget::ChangePrivateState(bool bPrivate)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		if (LobbyPlayerController)
			LobbyPlayerController->Server_ChangeAdvertiseState(!bPrivate);
	}
}

void ULobbyWidget::ChangeSelectedButton(EClassType ClassType)
{
	ClassBtns[SelectClass]->SetIsEnabled(true);
	SelectClass = ClassType;
	ClassBtns[SelectClass]->SetIsEnabled(false);
	
	SetPlayerCharacter(ClassType);
}

void ULobbyWidget::SetPlayerCharacter(EClassType ClassType)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		if (LobbyPlayerController)
		{
			UE_LOG(LogTemp, Log, TEXT("Request set character class : %d"), ClassType);
			
			LobbyPlayerController->Server_SetPlayerCharacter(ClassType);
		}
	}
}

void ULobbyWidget::ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
		case ETextCommit::Default:
		case ETextCommit::OnEnter:
			if (!Text.IsEmpty())
			{
				APlayerController* PlayerController = GetOwningPlayer();
				if (PlayerController)
				{
					ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
					if (LobbyPlayerController)
						LobbyPlayerController->Server_SendChatMessage(Text);
				}
				Chat_Etb->SetText(FText::GetEmpty());
			}
			break;

		case ETextCommit::OnUserMovedFocus:
		case ETextCommit::OnCleared:
			break;
	}
	FSlateApplication::Get().SetKeyboardFocus(Chat_Etb->TakeWidget());
}





void ULobbyWidget::UpdatePlayerNameList(const TArray<FString>& PlayerNameList)
{
	NumberOfPlayers = PlayerNameList.Num();

	UE_LOG(LogTemp, Log, TEXT("Widget : update player name list (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %d name : %s"), i, *PlayerNameList[i]);
		PlayerTexts[i]->SetText(FText::FromString(PlayerNameList[i]));
	}
}

void ULobbyWidget::UpdateReadyState(const TArray<bool>& PlayerReadyStateArray)
{
	NumberOfPlayers = PlayerReadyStateArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Widget : update player ready state (%d players)"), NumberOfPlayers);

	for(int i = 0; i < NumberOfPlayers; i++)
	{
		if(PlayerReadyStateArray[i]) PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Red);
		else PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Black);
	}
}


void ULobbyWidget::AddChatMessage(const FText& Text)
{
	UTextBlock* TextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	if (TextBlock)
	{
		TextBlock->SetText(Text);
		TextBlock->SetAutoWrapText(true);
		TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Black));

		if (Chat_Scr)
		{
			Chat_Scr->AddChild(TextBlock);
			Chat_Scr->ScrollToEnd();
		}
	}
}


void ULobbyWidget::ExitClicked()
{
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	
	if (EHGameInstance)
	{
		EHGameInstance->LeaveSession("JoinMainSession");
	}
}