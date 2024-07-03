// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSessionGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"


void AMainSessionGameSession::BeginPlay()
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
            if (PortNumber.Equals("7777"))
            {
                UE_LOG(LogTemp, Log, TEXT("Main Session Port : %s"), *PortNumber);
                CreateSession(PortNumber);
            }
            else UE_LOG(LogTemp, Error, TEXT("Main Session Port is not 7777"));

        }
        else UE_LOG(LogTemp, Error, TEXT("Port number error"));
    }
}

void AMainSessionGameSession::CreateSession(FString PortNumber)
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

            SessionSettings->bAllowInvites = false; // �ʴ� X
            //SessionSettings->bUsesStats = false; // ���� ��� ���� 
            //SessionSettings->bAntiCheatProtected = true; // ��Ƽ ġƮ ��ȣ ����

            // ���� �˻��� ���� ����� ���� �Ӽ� �߰�
            SessionSettings->Set("GameName", FString("EH2"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("PortNumber", PortNumber, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

            UE_LOG(LogTemp, Log, TEXT("Creating session..."));

            // ���� ���� �õ�
            if (!Session->CreateSession(0, SessionName, *SessionSettings))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
                Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
                CreateSessionDelegateHandle.Reset();
            }
        }
    }
}

void AMainSessionGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
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
                UE_LOG(LogTemp, Log, TEXT("Session: %s Created!"), *EOSSessionName.ToString());
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
            
            Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
            CreateSessionDelegateHandle.Reset();
        }
    }
}

//�������� �α����ϴ� ���� ���� (AGameModeBase::InitGame()���� �Ҹ�)
bool AMainSessionGameSession::ProcessAutoLogin()
{
    return true;
}

void AMainSessionGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
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
void AMainSessionGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful) //����
            {
                UE_LOG(LogTemp, Log, TEXT("Player registered in Session!"));
                NumberOfPlayersInSession++; //�÷��̾� �� ������
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to register player! (From Callback)"));

            // Clear and reset delegates
            Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
            RegisterPlayerDelegateHandle.Reset();
        }
    }
}

//NotifyLogout���� ȣ���
void AMainSessionGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
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

void AMainSessionGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player unregistered in Session!"));
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
void AMainSessionGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
    Super::NotifyLogout(ExitingPlayer); //UnregisterPlayer�� ȣ����

    if (IsRunningDedicatedServer())
    {
        NumberOfPlayersInSession--; //���� �÷��̾� �� --
    }
}