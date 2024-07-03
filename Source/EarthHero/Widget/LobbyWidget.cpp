// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>

bool ULobbyWidget::Initialize()
{
	Super::Initialize();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		if (LobbyPlayerController)
		{
			bHost = LobbyPlayerController->bHost;

			if (bHost)
				ReadyButton_Tb->SetText(FText::FromString("Game Start"));
			else
				ReadyButton_Tb->SetText(FText::FromString("Game Ready"));
		}
	}
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
	}

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
	
	return true;
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
	UE_LOG(LogTemp, Error, TEXT(" %d  %d "), NumberOfPlayers, PlayerNumber);
	
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
	UE_LOG(LogTemp, Error, TEXT(" %d  %d "), NumberOfPlayers, PlayerNumber);
	
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

void ULobbyWidget::ChangeSelectedButton(EClassType ClassType)
{
	for (int i = 0; i < NumberOfClass; i++)
	{
		ClassBtns[i]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	ClassBtns[ClassType]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

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