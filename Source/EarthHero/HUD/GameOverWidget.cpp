// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "EarthHero/PlayerController/GameOverPlayerController.h"

bool UGameOverWidget::Initialize()
{
	Super::Initialize();

	Exit_Btn->OnClicked.AddDynamic(this, &UGameOverWidget::ExitClicked);
	
	Chat_Etb->OnTextCommitted.AddDynamic(this, &UGameOverWidget::ChatTextCommitted);
	
	return true;
}

void UGameOverWidget::ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
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
				AGameOverPlayerController* GameOverPlayerController = Cast<AGameOverPlayerController>(PlayerController);
				if (GameOverPlayerController)
					GameOverPlayerController->Server_SendChatMessage(Text);
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

void UGameOverWidget::AddChatMessage(const FText& Text)
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


void UGameOverWidget::ExitClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController) PlayerController->ClientTravel("/Game/Maps/StartupMap", ETravelType::TRAVEL_Absolute);
}
