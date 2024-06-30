// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <EarthHero/GameMode/LobbyGameMode.h>



void ALobbyGameSession::BeginPlay()
{
    Super::BeginPlay();

    //dedicated ������ ���� ���� ����
    if (IsRunningDedicatedServer() && !bSessionExists)
    {
        //��Ʈ ��ȣ�� key������ �����
        FString PortNumber;
        UWorld* World = GetWorld();
        if (World)
        {
            PortNumber = FString::FromInt(World->URL.Port);
        }
        if (!PortNumber.IsEmpty())
        {
            if (!PortNumber.Equals("7777"))
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby port : %s"), *PortNumber);
                CreateSession(PortNumber);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Lobby is not 7777 port"));
            }
        }
        else UE_LOG(LogTemp, Error, TEXT("Port number error"));
    }
}

void ALobbyGameSession::CreateSession(FString PortNumber)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            CreateSessionDelegateHandle =
                Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleCreateSessionCompleted));

            TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
            SessionSettings->bIsDedicated = true; // Dedicated ���� ����
            SessionSettings->bIsLANMatch = false; // LAN ������ �ƴ�
            SessionSettings->NumPublicConnections = MaxNumberOfPlayersInSession; // �ִ� ���� �ο� ����
            SessionSettings->NumPrivateConnections = 0; // ����� ���� �ο� ����
            SessionSettings->bShouldAdvertise = true; // ���� ����
            SessionSettings->bUsesPresence = false; // Presence�� ������� ����
            SessionSettings->bUseLobbiesIfAvailable = false; // �κ� ��� ����

            SessionSettings->bAllowInvites = true; // �ʴ� ���
            SessionSettings->bAllowJoinInProgress = true; // ���� ���� ���ǿ� ���� �ȵ�
            //SessionSettings->bUsesStats = false; // ���� ��� ���� 
            //SessionSettings->bAntiCheatProtected = true; // ��Ƽ ġƮ ��ȣ ����

            // ���� �˻��� ���� ����� ���� �Ӽ� �߰�
            SessionSettings->Set("GameName", FString("EH2"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("PortNumber", PortNumber, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            //SessionSettings->Set("LobbyType", FString("Public"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); //��...

            UE_LOG(LogTemp, Log, TEXT("Creating lobby..."));

            // ���� ���� �õ�
            if (!Session->CreateSession(0, SessionName, *SessionSettings))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
                Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
                CreateSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                bSessionExists = true;
                UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *EOSSessionName.ToString());
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));

            // Clear our handle and reset the delegate. 
            Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
            CreateSessionDelegateHandle.Reset();
        }
    }
}

//�������� �α����ϴ� ���� ���� (AGameModeBase::InitGame()���� �Ҹ�)
bool ALobbyGameSession::ProcessAutoLogin()
{
    return true;
}

void ALobbyGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
    Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

    if (IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

            if (Session.IsValid())
            {
                NewPlayerPlayerController = NewPlayer;

                RegisterPlayerDelegateHandle =
                    Session->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
                        this,
                        &ThisClass::HandleRegisterPlayerCompleted));

                //���ǿ� �÷��̾� ���
                if (!Session->RegisterPlayer(SessionName, *UniqueId, false))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Register Player!"));
                    Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
                    RegisterPlayerDelegateHandle.Reset();
                }
            }
        }
    }
}

//�÷��̾� ��� ���
//PlayerIds�� �¶��� ���񽺿� ����� ��� �÷��̾� id�� �ǹ�
void ALobbyGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful) //����
            {
                UE_LOG(LogTemp, Log, TEXT("Player registered in Lobby!"));
                NumberOfPlayersInSession++; //�÷��̾� �� ������
                UE_LOG(LogTemp, Error, TEXT("NumberOfPlayersInSession = %d"), NumberOfPlayersInSession); //�׽�Ʈ

                //ù�÷��̾� = ����
                if (NumberOfPlayersInSession == 1)
                {
                    //ù ��° �÷��̾ ����
                    HostPlayerId = PlayerIds[0]; //�̰� �÷��̾� ��Ʈ�ѷ� �������� ���� �ʿ�
                    UE_LOG(LogTemp, Log, TEXT("Host Assigment..."), *HostPlayerId->ToString());

                    //Ŭ���̾�Ʈ���� ���� ������ �ο�
                    HostAssignment(NewPlayerPlayerController);
                }

                // playerreadystate�� ���ο� �÷��̾� ���� ���� false �߰�
                ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
                if (LobbyGameMode)
                {
                    LobbyGameMode->AddPlayerReadyState(NewPlayerPlayerController);
                }
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to register player! (From Callback)"));

            // Clear and reset delegates
            Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
            RegisterPlayerDelegateHandle.Reset();
        }
    }
}

//���ǿ� �÷��̾� ���� �� ���� ȣ���
void ALobbyGameSession::StartSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            StartSessionDelegateHandle =
                Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleStartSessionCompleted));

            //���� ���� �õ�
            if (!Session->StartSession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to start lobby!"));
                Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
                StartSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby Started!"));

                FTimerHandle UnusedHandle;
                GetWorldTimerManager().SetTimer(UnusedHandle, this, &ALobbyGameSession::ChangeMap, 5.0f, false);
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to start lobby!! (From Callback)"));

            Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
            StartSessionDelegateHandle.Reset();
        }
    }
}

void ALobbyGameSession::ChangeMap()
{
    // �� ��ȯ //���� ��������
    GetWorld()->ServerTravel(InGameMap, true);
}

//NotifyLogout���� ȣ���
void ALobbyGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
    Super::UnregisterPlayer(ExitingPlayer);

    if (IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            //IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
            if (Session.IsValid())
            {
                // If the player leaves ungracefully this could be null
                if (ExitingPlayer->PlayerState)
                {
                    UnregisterPlayerDelegateHandle =
                        Session->AddOnUnregisterPlayersCompleteDelegate_Handle(FOnUnregisterPlayersCompleteDelegate::CreateUObject(
                            this,
                            &ThisClass::HandleUnregisterPlayerCompleted));

                    // ���ǿ��� �÷��̾� ����
                    if (ExitingPlayer && !Session->UnregisterPlayer(SessionName, *ExitingPlayer->PlayerState->UniqueId))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
                        Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
                        UnregisterPlayerDelegateHandle.Reset();
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
                    Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
                    UnregisterPlayerDelegateHandle.Reset();
                }
            }
        }
    }
}

//PlayerIds�� �¶��� ���꿡�� ���� �÷��̾� id��
void ALobbyGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player unregistered in Lobby!"));

                // ������ �������� Ȯ��
                for (const FUniqueNetIdRef& PlayerId : PlayerIds)
                {
                    if (HostPlayerId == PlayerId)
                    {
                        UE_LOG(LogTemp, Log, TEXT("Host has left the lobby"));

                        NewHostFind();

                        break;
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to unregister player! (From Callback)"));
            }
            Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
            UnregisterPlayerDelegateHandle.Reset();
        }
    }
}

//�÷��̾ ���� ������ ���� �� �ڵ����� ȣ���
void ALobbyGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
    Super::NotifyLogout(ExitingPlayer); //UnregisterPlayer�� ȣ����


    if (IsRunningDedicatedServer())
    {
        NumberOfPlayersInSession--; //���� �÷��̾� �� --

        //���ǿ� �ƹ��� ���ٸ� ���� ����
        if (NumberOfPlayersInSession == 0)
        {
            EndSession();
        }
    }
}

void ALobbyGameSession::EndSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            EndSessionDelegateHandle =
                Session->AddOnEndSessionCompleteDelegate_Handle(FOnEndSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleEndSessionCompleted));

            //���� ���� �õ�
            if (!Session->EndSession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to end session!"));
                Session->ClearOnEndSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
                EndSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby ended!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to end lobby! (From Callback)"));
            }

            Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle);
            EndSessionDelegateHandle.Reset();
        }
    }
}

//���� �ı� �� ���� �� Ŭ���̾�Ʈ �� �� ȣ���
void ALobbyGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    DestroySession();
}

// ��� �÷��̾ ���� ������ ���� �� ȣ���
void ALobbyGameSession::DestroySession()
{
    // ������ �ı��Ͽ� �鿣�忡�� ����
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            DestroySessionDelegateHandle =
                Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleDestroySessionCompleted));

            //���� ���� �õ�
            if (!Session->DestroySession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy lobby."));
                Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
                DestroySessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                bSessionExists = false;
                UE_LOG(LogTemp, Log, TEXT("Destroyed lobby succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy lobby."));
            }

            Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
            DestroySessionDelegateHandle.Reset();
        }
    }
}

void ALobbyGameSession::ChangeAdvertiseState(bool bAdvertise)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            // ���� ���� ��������
            FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
            if (SessionSettings)
            {
                // ���� ���� ����
                SessionSettings->bShouldAdvertise = bAdvertise;

                UpdateSessionDelegateHandle =
                    Session->AddOnUpdateSessionCompleteDelegate_Handle(FOnUpdateSessionCompleteDelegate::CreateUObject(
                        this,
                        &ALobbyGameSession::HandleUpdateSessionCompleted));


                // ���� ������Ʈ �õ�
                if (!Session->UpdateSession(SessionName, *SessionSettings, true))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to update Lobby"));
                    Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
                    UpdateSessionDelegateHandle.Reset();
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No session settings found for session: %s"), *SessionName.ToString());
            }
        }
    }
}

void ALobbyGameSession::HandleUpdateSessionCompleted(FName EOSSessionName, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Update lobby succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to update lobby."));
            }

            Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
            UpdateSessionDelegateHandle.Reset();
        }
    }
}


void ALobbyGameSession::NewHostFind()
{
    //�̿ϼ�
    
    // �ε��� 0�� �÷��̾� ��Ʈ�ѷ��� ����� (�׽�Ʈ x, �ȵǸ� ���� ������Ʈ �������)

    //�ƴ���.. const TArray<FUniqueNetIdRef>& PlayerIds�� �����ϴµ�...
    //HostPlayerId ���� �ʿ�

    
    APlayerController* NewHostPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (NewHostPlayerController)
    {
        ALobbyPlayerController* NewHostLobbyPlayerController = Cast<ALobbyPlayerController>(NewHostPlayerController);

        if (NewHostLobbyPlayerController)
        {
            APlayerState* PlayerState = NewHostLobbyPlayerController->PlayerState;
            
            UE_LOG(LogTemp, Log, TEXT("Host Assignment..."));

            if (PlayerState && PlayerState->GetUniqueId().IsValid())
            {
                HostPlayerId = PlayerState->GetUniqueId(); //�׽�Ʈ x
                HostAssignment(NewHostPlayerController);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to get unique ID"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("player controller 0 is not vaild"));
    }
}

//Ŭ���̾�Ʈ�� ���Դµ� ù��° �÷��̾��� host �ο� 
void ALobbyGameSession::HostAssignment(APlayerController* HostPlayer)
{
    if (HostPlayer)
    {
        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(HostPlayer);
        if (LobbyPlayerController)
        {
            UE_LOG(LogTemp, Log, TEXT("Host Assigment : %s"), *HostPlayerId->ToString());
            LobbyPlayerController->bHost = true;
        }
    }
}