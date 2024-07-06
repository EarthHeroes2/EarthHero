// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
//#include "MultiplayerSessionsSubsystem.h"
#include "LobbyRowWidget.h"
#include "../EHGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



UMainMenuWidget::UMainMenuWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//친구 초대 row 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyRowWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Lobby_Row.WBP_Lobby_Row_C'"));
	if (LobbyRowWidgetAsset.Succeeded())
	{
		LobbyRowWidgetClass = LobbyRowWidgetAsset.Class;
	}
}
	
void UMainMenuWidget::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	/*
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}*/
}

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Play_Btn)
	{
		Play_Btn->OnClicked.AddDynamic(this, &ThisClass::Play_BtnClicked);
	}
	if (Join_Btn)
	{
		Join_Btn->OnClicked.AddDynamic(this, &ThisClass::Join_BtnClicked);
	}
	if (Exit_Btn)
	{
		Exit_Btn->OnClicked.AddDynamic(this, &ThisClass::Exit_BtnClicked);
	}

	if(LobbyList_Btn)
	{
		LobbyList_Btn->OnClicked.AddDynamic(this, &ThisClass::LobbyListBtnClicked);
	}
	if(FindLobby_Btn)
	{
		FindLobby_Btn->OnClicked.AddDynamic(this, &ThisClass::FindLobbyBtnClicked);
	}

	return true;
}

void UMainMenuWidget::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Created session, entering game!"));
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		Play_Btn->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			//MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		Join_Btn->SetIsEnabled(true);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Session Found!"));
	}
}

void UMainMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMainMenuWidget::OnDestroySession(bool bWasSuccessful)
{
}

void UMainMenuWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMainMenuWidget::Play_BtnClicked()
{
	//�ο� 0�� �κ�� ����
	UE_LOG(LogTemp, Log, TEXT("Create Lobby Clicked"));
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	if (EHGameInstance)
	{
		EHGameInstance->IsCheckedPrivate = Private_Cb->IsChecked();
		EHGameInstance->LeaveSession("CreateLobby");
	}

	/* ���� ��
	Play_Btn->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}*/
}

void UMainMenuWidget::Join_BtnClicked()
{
	//���� public �κ� ����
	UE_LOG(LogTemp, Log, TEXT("Join Lobby Clicked"));
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	if (EHGameInstance)
	{
		//EHGameInstance->IsCheckedPrivate = true; //�׽�Ʈ������ ���а���
		//�κ� ���� �� ���� ������. (�̸� ���� ����)
		EHGameInstance->LeaveSession("JoinLobby");
	}

	/* ���� ��
	Join_Btn->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}*/
}

void UMainMenuWidget::Exit_BtnClicked()
{
	// Get the current player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Ensure the PlayerController is valid
	if (PlayerController)
	{
		// Call the QuitGame function from the UKismetSystemLibrary
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

void UMainMenuWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UMainMenuWidget::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}



void UMainMenuWidget::LobbyListBtnClicked()
{
	if(LobbyList_Bd)
	{
		if(LobbyList_Bd->GetVisibility() == ESlateVisibility::Collapsed)
		{
			LobbyList_Bd->SetVisibility(ESlateVisibility::Visible);

			FindLobbys();
		}
		else
			LobbyList_Bd->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenuWidget::FindLobbyBtnClicked()
{
	FindLobbys();
}


void UMainMenuWidget::FindLobbys()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

            Search->QuerySettings.SearchParams.Empty();
            Search->MaxSearchResults = 1000;
            Search->bIsLanQuery = false;
        	
            FindSessionsDelegateHandle =
                Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleFindSessionsCompleted,
                    Search));

            UE_LOG(LogTemp, Log, TEXT("Finding Lobby"));

            if (!Session->FindSessions(0, Search))
            {
                UE_LOG(LogTemp, Warning, TEXT("Find lobby failed"));
            }
        }
    }
}

void UMainMenuWidget::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Found lobby : %d"), Search->SearchResults.Num());

            	TArray<FOnlineSessionSearchResult> FindLobbyList;

                bool bIsFind = false;

                for (FOnlineSessionSearchResult SessionInSearchResult : Search->SearchResults)
                {
                    FString GameName;
                    bool bKeyValueFound1 = SessionInSearchResult.Session.SessionSettings.Get("GameName", GameName);

                    FString PortNumber;
                    bool bKeyValueFound2 = SessionInSearchResult.Session.SessionSettings.Get("PortNumber", PortNumber);
                    
                    int32 NumberOfJoinedPlayers;
                    bool bKeyValueFound3 = SessionInSearchResult.Session.SessionSettings.Get("NumberOfJoinedPlayers", NumberOfJoinedPlayers);

                    bool bAdvertise;
                    bool bKeyValueFound4 = SessionInSearchResult.Session.SessionSettings.Get("Advertise", bAdvertise);

                    if (bKeyValueFound1 && bKeyValueFound2 && bKeyValueFound3 && bKeyValueFound4)
                    {
                        if (GameName == "EH2" && bAdvertise)
                        {
                        	UE_LOG(LogTemp, Log, TEXT("Valid lobby : %s, %d"), *PortNumber, NumberOfJoinedPlayers);

                        	bIsFind = true;

                        	//리스트 추가
                        	FindLobbyList.Add(SessionInSearchResult);
                        }
                    }
                }
                //들어갈 로비를 찾지 못함
                if (!bIsFind && GEngine) GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("No lobby")));

            	UpdateLobbyList(FindLobbyList);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Find lobbys failed."));
                if(GEngine)
                    GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Find lobbys failed!")));
            }

            Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
            FindSessionsDelegateHandle.Reset();
        }
    }
}

void UMainMenuWidget::UpdateLobbyList(TArray<FOnlineSessionSearchResult> FindLobbyList)
{
	TArray<FString> FindLobbyIdList;
	
	for (FOnlineSessionSearchResult FindLobby : FindLobbyList)
	{
		FString LobbyId = FindLobby.GetSessionIdStr();
		
		FindLobbyIdList.Add(LobbyId);
		
		int LobbyIndex = LobbyIdList.IndexOfByKey(LobbyId);

		//이미 존재하는 정보라면 업데이트만
		if(LobbyIndex != INDEX_NONE)
		{
			LobbyRowList[LobbyIndex]->UpdateLobbyInfo(FindLobby);
		}
		else //아니라면 새로 추가
		{
			ULobbyRowWidget* LobbyRowWidget = Cast<ULobbyRowWidget>(CreateWidget(GetWorld(), LobbyRowWidgetClass));
			if(LobbyRowWidget)
			{
				LobbyIdList.Add(FindLobby.GetSessionIdStr());
				LobbyRowList.Add(LobbyRowWidget);
				
				LobbyRowWidget->UpdateLobbyInfo(FindLobby);
			}
			LobbyList_Vb->AddChild(LobbyRowWidget);
		}
	}

	//기존 정보를 살펴봄
	int i = 0;
	for (FString LobbyId : LobbyIdList)
	{
		int FindLobbyIndex = FindLobbyIdList.IndexOfByKey(LobbyId);

		//새정보에 기존 정보가 존재하지 않으면 삭제
		if(FindLobbyIndex == INDEX_NONE)
		{
			LobbyIdList.RemoveAt(i);
			LobbyRowList.RemoveAt(i);
			//명시적 제거 없나?
		}
		else i++;
	}
	
}