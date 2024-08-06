// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

		Server_ClientReady();

		//로딩화면 제거
		UEHGameInstance* EHGameinstance = Cast<UEHGameInstance>(GetGameInstance());
		if(EHGameinstance) EHGameinstance->RemoveSeamlessLoadingScreen();
	}
	else
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
		if(EHGameInstance)
			bGameClear = EHGameInstance->bGameClear;
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

void AGameOverPlayerController::Server_ClientReady_Implementation()
{
	//게임 결과와 여러 통계를 보여줌?
	Client_InitSetting(bGameClear);
}

void AGameOverPlayerController::Client_InitSetting_Implementation(bool bClear)
{
	if (GameOverWidget && GameOverWidget->GameResult_Tb)
	{
		if(bGameClear) GameOverWidget->GameResult_Tb->SetText(FText::FromString(FString("Clear")));
		else GameOverWidget->GameResult_Tb->SetText(FText::FromString(FString("Lose")));
	}
}