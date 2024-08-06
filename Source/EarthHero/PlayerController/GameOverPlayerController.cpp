// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/GameMode/GameOverGameMode.h"
#include "EarthHero/Menu/GameOverWidget.h"
#include "GameFramework/PlayerState.h"

AGameOverPlayerController::AGameOverPlayerController()
{
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetAsset(TEXT("UserWidget'/Game/Blueprints/HUD/BP_GameOverHUD.BP_GameOverHUD_C'"));
		if (GameOverWidgetAsset.Succeeded())
		{
			GameOverWidgetClass = GameOverWidgetAsset.Class;
		}
	}
}

void AGameOverPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		if (GameOverWidgetClass)
		{
			GameOverWidget = Cast<UGameOverWidget>(CreateWidget(GetWorld(), GameOverWidgetClass));
		
			if (GameOverWidget)
			{
				GameOverWidget->AddToViewport();
				bShowMouseCursor = true;

				FInputModeUIOnly InputMode;
				SetInputMode(InputMode);
			}
		}

		//로딩화면 제거
		UEHGameInstance* EHGameinstance = Cast<UEHGameInstance>(GetGameInstance());
		if(EHGameinstance) EHGameinstance->RemoveSeamlessLoadingScreen();
	}
}

void AGameOverPlayerController::Server_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Send a chat message"));

	FText PlayerName = FText::FromString(PlayerState->GetPlayerName());
	FText ChatText = FText::Format(FText::FromString("[{0}]: {1}"), PlayerName, Text);

	AGameOverGameMode* GameOverGameMode = Cast<AGameOverGameMode>(GetWorld()->GetAuthGameMode());
	if (GameOverGameMode)
	{
		GameOverGameMode->SendChatMessage(ChatText);
	}
}

void AGameOverPlayerController::Client_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Receive a chat message"));
	if (GameOverWidget)
	{
		GameOverWidget->AddChatMessage(Text);
	}
}