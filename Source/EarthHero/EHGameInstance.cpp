#include "EHGameInstance.h"

#include "AudioDevice.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Options.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

UEHGameInstance::UEHGameInstance()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_GameTable(TEXT("/Game/Data/Character/DT_CharacterStat.DT_CharacterStat"));
    if (DT_GameTable.Succeeded())
    {
        CharacterStatDataTable = DT_GameTable.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<USoundMix> SoundMixFinder(TEXT("/Game/Sounds/MainSoundMix.MainSoundMix"));
    if (SoundMixFinder.Succeeded())
    {
        MainSoundMix = SoundMixFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> MasterSoundClassFinder(TEXT("/Game/Sounds/MasterVolume.MasterVolume"));
    if (MasterSoundClassFinder.Succeeded())
    {
        MasterVolumeSoundClass = MasterSoundClassFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> BackgroundSoundClassFinder(TEXT("/Game/Sounds/BGMVolume.BGMVolume"));
    if (BackgroundSoundClassFinder.Succeeded())
    {
        BackgroundVolumeSoundClass = BackgroundSoundClassFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundClass> SFXSoundClassFinder(TEXT("/Game/Sounds/SFXVolume.SFXVolume"));
    if (SFXSoundClassFinder.Succeeded())
    {
        SFXVolumeSoundClass = SFXSoundClassFinder.Object;
    }
}

FStatStructure* UEHGameInstance::GetStatStructure(FName HeroName) const
{
    if (!CharacterStatDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Such DataTable"));
        return nullptr;
    }
    return  CharacterStatDataTable->FindRow<FStatStructure>(HeroName, TEXT(""));
}

void UEHGameInstance::Init()
{
    Super::Init();
    LoadSettings();
}

void UEHGameInstance::FindSessions(FString Reason)
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

            FindSessionReason = Reason;

            FindSessionsDelegateHandle =
                Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleFindSessionsCompleted,
                    Search));

            UE_LOG(LogTemp, Log, TEXT("Finding session."));

            if (!Session->FindSessions(0, Search))
            {
                UE_LOG(LogTemp, Warning, TEXT("Find session failed"));
            }
        }
    }
}

void UEHGameInstance::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Found session : %d"), Search->SearchResults.Num());

                bool bIsFind = false;

                for (auto SessionInSearchResult : Search->SearchResults)
                {
                    FString GameName;
                    bool bKeyValueFound1 = SessionInSearchResult.Session.SessionSettings.Get("GameName", GameName);

                    FString PortNumber;
                    bool bKeyValueFound2 = SessionInSearchResult.Session.SessionSettings.Get("PortNumber", PortNumber);

                    //NumOpenPublicConnections가 업데이트 안되서 대체
                    //int32 NumberOfJoinedPlayers;
                    //bool bKeyValueFound3 = SessionInSearchResult.Session.SessionSettings.Get("NumberOfJoinedPlayers", NumberOfJoinedPlayers);

                    if (bKeyValueFound1 && bKeyValueFound2)
                    {
                        UE_LOG(LogTemp, Log, TEXT("Test session : %s, %s, %d, %d"), *FindSessionReason, *PortNumber, SessionInSearchResult.Session.SessionSettings.NumPublicConnections, SessionInSearchResult.Session.NumOpenPublicConnections);
                        
                        if (GameName == "EH2" &&
                                (
                                    (FindSessionReason == "JoinMainSession" && PortNumber == "7777") ||
                                    (FindSessionReason == "JoinLobby" && PortNumber != "7777") ||
                                    (FindSessionReason == "CreateLobby" && PortNumber != "7777")
                                )
                            )
                        {
                            UE_LOG(LogTemp, Log, TEXT("Valid session : %s, %s"), *FindSessionReason, *PortNumber);

                            if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
                            {
                                SessionToJoin = &SessionInSearchResult;
                                if (SessionToJoin)
                                {
                                    bIsFind = true;
                                    JoinSession();
                                    break;
                                }
                            }
                        }
                    }
                }

                //���� ã�� �κ� ���ٸ�
                if (!bIsFind && GEngine)
                {
                    if (FindSessionReason == "JoinMainSession")
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("EH2 server connection failed")));
                        //������? ��⿭?
                    }
                    else if (FindSessionReason == "JoinLobby")
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("No lobby to participate")));
                    }
                    else if (FindSessionReason == "CreateLobby")
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Unable to create lobby (server full)")));
                    }

                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Find Sessions failed."));
                if(GEngine)
                    GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Find Sessions failed!")));
            }

            Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
            FindSessionsDelegateHandle.Reset();
        }
    }
}

void UEHGameInstance::JoinSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            JoinSessionDelegateHandle =
                Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleJoinSessionCompleted));

            UE_LOG(LogTemp, Log, TEXT("Joining session."));

            if (!Session->JoinSession(0, "MainSession", *SessionToJoin))
            {
                UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
            }
        }
    }
}

void UEHGameInstance::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (Result == EOnJoinSessionCompleteResult::Success)
            {
                UE_LOG(LogTemp, Log, TEXT("Joined session."));
                if (GEngine)
                {
                    if (!Session->GetResolvedConnectString(SessionName, ConnectString))
                    {
                        UE_LOG(LogTemp, Error, TEXT("Could not get connect string."));
                        return;
                    }

                    if (GEngine)
                        GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *ConnectString));

                    JoinedSessionName = SessionName;

                    FURL DedicatedServerURL(nullptr, *ConnectString, TRAVEL_Absolute);
                    FString DedicatedServerJoinError;
                    auto DedicatedServerJoinStatus = GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, DedicatedServerJoinError);
                    if (DedicatedServerJoinStatus == EBrowseReturnVal::Failure)
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to browse for dedicated server. Error is: %s"), *DedicatedServerJoinError);
                    }
                }
            }
            Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
            JoinSessionDelegateHandle.Reset();
        }
    }
}

void UEHGameInstance::LeaveSession(FString Reason)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            DestroySessionCompleteDelegatesHandle =
                Session->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::DestroySessionComplete);

            LeaveSessionReason = Reason;

            UE_LOG(LogTemp, Log, TEXT("Leave session"));

            if (!Session->DestroySession(JoinedSessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to DestroySession : %s"), *JoinedSessionName.ToString());
            }
        }
    }
}

void UEHGameInstance::DestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            //일단 성공이든 실패든 세션 찾고 들어감
            if (bWasSuccessful)
            {
                FindSessions(LeaveSessionReason);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session"));
                FindSessions(LeaveSessionReason);
            }

            Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegatesHandle);
            DestroySessionCompleteDelegatesHandle.Reset();
        }
    }
}

void UEHGameInstance::SaveSettings()
{
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("Settings.sav");

    FString SaveString;
    SaveString += FString::FromInt(ResolutionWidth) + TEXT("\n"); // Save width
    SaveString += FString::FromInt(ResolutionHeight) + TEXT("\n"); // Save height
    SaveString += FString::FromInt(ScreenMode) + TEXT("\n");
    SaveString += FString::FromInt(MaxFrame) + TEXT("\n");
    SaveString += bVSyncEnabled ? TEXT("1\n") : TEXT("0\n");
    SaveString += FString::FromInt(OverallQuality) + TEXT("\n");
    SaveString += FString::FromInt(AntiAliasing) + TEXT("\n");
    SaveString += FString::FromInt(PostProcessing) + TEXT("\n");
    SaveString += FString::SanitizeFloat(MasterVolume) + TEXT("\n");
    SaveString += FString::SanitizeFloat(BackgroundVolume) + TEXT("\n");
    SaveString += FString::SanitizeFloat(SFXVolume) + TEXT("\n");
    SaveString += FString::SanitizeFloat(MouseSensitivity) + TEXT("\n");

    FFileHelper::SaveStringToFile(SaveString, *SavePath);
}

void UEHGameInstance::LoadSettings()
{
    FString SavePath = FPaths::ProjectSavedDir() + TEXT("Settings.sav");
    FString LoadString;

    if (FFileHelper::LoadFileToString(LoadString, *SavePath))
    {
        TArray<FString> ParsedStrings;
        LoadString.ParseIntoArray(ParsedStrings, TEXT("\n"), true);

        if (ParsedStrings.Num() >= 12) // Expecting 12 lines now (width + height)
        {
            ResolutionWidth = FCString::Atoi(*ParsedStrings[0]);
            ResolutionHeight = FCString::Atoi(*ParsedStrings[1]);
            ScreenMode = FCString::Atoi(*ParsedStrings[2]);
            MaxFrame = FCString::Atoi(*ParsedStrings[3]);
            bVSyncEnabled = FCString::Atoi(*ParsedStrings[4]) == 1;
            OverallQuality = FCString::Atoi(*ParsedStrings[5]);
            AntiAliasing = FCString::Atoi(*ParsedStrings[6]);
            PostProcessing = FCString::Atoi(*ParsedStrings[7]);
            MasterVolume = FCString::Atof(*ParsedStrings[8]);
            BackgroundVolume = FCString::Atof(*ParsedStrings[9]);
            SFXVolume = FCString::Atof(*ParsedStrings[10]);
            MouseSensitivity = FCString::Atof(*ParsedStrings[11]);

            // Apply the loaded settings
            UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
            if (UserSettings)
            {
                EWindowMode::Type WindowMode = EWindowMode::Windowed;
                switch (ScreenMode)
                {
                case 0:
                    WindowMode = EWindowMode::Fullscreen;
                    break;
                case 1:
                    WindowMode = EWindowMode::WindowedFullscreen;
                    break;
                case 2:
                default:
                    WindowMode = EWindowMode::Windowed;
                    break;
                }
                UserSettings->SetFullscreenMode(WindowMode);

                // Set resolution directly from the loaded values
                UserSettings->SetScreenResolution(FIntPoint(ResolutionWidth, ResolutionHeight));

                UserSettings->SetVSyncEnabled(bVSyncEnabled);
                UserSettings->SetOverallScalabilityLevel(OverallQuality);
                UserSettings->SetAntiAliasingQuality(AntiAliasing);
                UserSettings->SetPostProcessingQuality(PostProcessing);
                UserSettings->SetFrameRateLimit(MaxFrame);
                UserSettings->ApplySettings(false);
            }

            // Apply volume
            if (GEngine && GEngine->GetMainAudioDevice())
            {
                GEngine->GetMainAudioDevice()->SetSoundMixClassOverride(MainSoundMix, MasterVolumeSoundClass, MasterVolume, 1.0f, 0.0f, true);
                GEngine->GetMainAudioDevice()->SetSoundMixClassOverride(MainSoundMix, BackgroundVolumeSoundClass, BackgroundVolume, 1.0f, 0.0f, true);
                GEngine->GetMainAudioDevice()->SetSoundMixClassOverride(MainSoundMix, SFXVolumeSoundClass, SFXVolume, 1.0f, 0.0f, true);
                UGameplayStatics::PushSoundMixModifier(this, MainSoundMix);
            }
            
            /* Apply mouse sensitivity
            InputSubsystem->SetMouseSensitivity(MouseSensitivity); */
        }
    }
}


