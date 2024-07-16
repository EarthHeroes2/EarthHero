// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>

#include "FriendRowWidget.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "EarthHero/EHGameInstance.h"

#include "steam/steam_api.h"

ULobbyWidget::ULobbyWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//친구 초대 row 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> FriendRowWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Friend_Row.WBP_Friend_Row_C'"));
	if (FriendRowWidgetAsset.Succeeded())
	{
		FriendRowWidgetClass = FriendRowWidgetAsset.Class;
	}
}

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


	UWorld* World = GetWorld();
	if (World)
	{
		
		World->GetTimerManager().SetTimer(ReadFriendsListTimerHandle, this, &ThisClass::ReadFriendsList, 4.0f, true);
	}

	return true;
}

void ULobbyWidget::ReadFriendsList()
{
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
	}
}

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
					TArray<CSteamID> FindFriendSteamIds;
					
					//친구 리스트 얻고
					TArray<TSharedRef<FOnlineFriend>> FriendList;
					Friends->GetFriendsList(LocalUserNum, ListName, FriendList);

					UE_LOG(LogTemp, Log, TEXT("Friend List : %d"), FriendList.Num());
					
					//하나씩 살피며
					for (TSharedRef<FOnlineFriend> Friend : FriendList)
					{
						CSteamID FriendSteamId(*(uint64*)Friend->GetUserId()->GetBytes());
						
						FindFriendSteamIds.Add(FriendSteamId);

						bool bFriendOnline = SteamFriends()->GetFriendPersonaState(FriendSteamId) == k_EPersonaStateOnline;
						
						int FriendRowIndex = FriendSteamIds.IndexOfByKey(FriendSteamId);
						
						//이미 관리 중인 친구라면
						if(FriendRowIndex != INDEX_NONE)
						{
							//해당 위젯 정보 업데이트
							if (FriendRowWidgets[FriendRowIndex]) FriendRowWidgets[FriendRowIndex]->UpdateFriendInfo(Friend, bFriendOnline);
						}
						//새로운 친구라면
						else
						{
							FriendSteamIds.Add(FriendSteamId);
							
							//위젯 생성
							UFriendRowWidget* FriendRowWidget = Cast<UFriendRowWidget>(CreateWidget(GetWorld(), FriendRowWidgetClass));
							FriendRowWidgets.Add(FriendRowWidget);
						
							if (FriendRowWidget)
							{
								//위젯에 친구 정보 넘김
								FriendRowWidget->UpdateFriendInfo(Friend, bFriendOnline);
						
								Friend_Scr->AddChild(FriendRowWidget);
							}
						}
					}

					for (int i = 0; i < FriendSteamIds.Num(); )
					{
						int FindLobbyIndex = FindFriendSteamIds.IndexOfByKey(FriendSteamIds[i]);

						//새정보에 기존 정보가 존재하지 않으면 삭제
						if(FindLobbyIndex == INDEX_NONE)
						{
							FriendRowWidgets[i]->RemoveFromParent();
			
							FriendSteamIds.RemoveAt(i);
							FriendRowWidgets.RemoveAt(i);
						}
						else i++;
					}
				}
			}
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("Failed to read Firends list"));
}

void ULobbyWidget::HostAssignment(bool bHostAssignment)
{
	bHost = bHostAssignment;
	
	if (bHost)
		ReadyButton_Tb->SetText(FText::FromString("Game Start"));
	else
		ReadyButton_Tb->SetText(FText::FromString("Ready"));
	
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

	//방장이 아니면 레디 버튼으로 작동. 클래스 버튼 작동 on/off
	if(!bHost) Class_Vb->SetIsEnabled(!(Class_Vb->GetIsEnabled()));

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);

		if (LobbyPlayerController)
		{
			//토글 형태로 작동함
			LobbyPlayerController->Server_ClientReadyButtonClicked();
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
	ChangeSelectedButton(Archer);
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
				if(Chat_Etb) Chat_Etb->SetText(FText::GetEmpty());
			}
			break;

		case ETextCommit::OnUserMovedFocus:
		case ETextCommit::OnCleared:
			break;
	}
	if(Chat_Etb) Chat_Etb->SetKeyboardFocus();
}

void ULobbyWidget::AddChatMessage(const FText& Text)
{
	UTextBlock* TextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	if (TextBlock)
	{
		FSlateFontInfo FontInfo = TextBlock->GetFont();
        FontInfo.Size = 16;
		
		TextBlock->SetFont(FontInfo);
		TextBlock->SetText(Text);
		TextBlock->SetAutoWrapText(true);
		TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));

		if (Chat_Scr)
		{
			Chat_Scr->AddChild(TextBlock);
			Chat_Scr->ScrollToEnd();
		}
	}
}



void ULobbyWidget::UpdatePlayerNameList(const TArray<FString>& PlayerNameList)
{
	int i;
	
	NumberOfPlayers = PlayerNameList.Num();

	UE_LOG(LogTemp, Log, TEXT("Widget : update player name list (%d players)"), NumberOfPlayers);

	for (i = 0; i < NumberOfPlayers; i++)
	{
		PlayerTexts[i]->SetText(FText::FromString(PlayerNameList[i]));
	}
	for (; i < MaxNumberOfPlayers; i++)
	{
		PlayerTexts[i]->SetText(FText::GetEmpty());
	}
}

void ULobbyWidget::UpdateReadyState(const TArray<bool>& PlayerReadyStateArray)
{
	int i;
	
	NumberOfPlayers = PlayerReadyStateArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Widget : update player ready state (%d players)"), NumberOfPlayers);

	//방장은 첫 칸. 방장은 초록색 (임시)
	PlayerTexts[0]->SetColorAndOpacity(FLinearColor::Green);
	
	for(i = 1; i < NumberOfPlayers; i++)
	{
		if(PlayerReadyStateArray[i]) PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Red);
		else PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Black);
	}
	for (; i < MaxNumberOfPlayers; i++)
	{
		PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Black);
	}
}


void ULobbyWidget::ExitClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		//이것을 이용하여 바로 나감
		PlayerController->ClientTravel("/Game/Maps/StartupMap", ETravelType::TRAVEL_Absolute);
	}
}