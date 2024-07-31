#include "EHGameInstance.h"

#include "AudioDevice.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
//#include "Options.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Stat/Effect/EffectBase.h"
#include "Stat/Structure/EffectStructure.h"

constexpr int32 DefaultResolutionWidth = 1920;
constexpr int32 DefaultResolutionHeight = 1080;
constexpr int32 DefaultScreenMode = 0; // Fullscreen
constexpr int32 DefaultMaxFrame = 60;
constexpr bool DefaultVSyncEnabled = false;
constexpr int32 DefaultOverallQuality = 2; // Medium
constexpr int32 DefaultAntiAliasing = 2; // Medium
constexpr int32 DefaultPostProcessing = 2; // Medium
constexpr float DefaultVolume = 0.5f;
constexpr float DefaultMouseSensitivity = 0.5f;


UEHGameInstance::UEHGameInstance()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_GameTable(TEXT("/Game/Data/Character/DT_CharacterStat.DT_CharacterStat"));
    if (DT_GameTable.Succeeded())
    {
        CharacterStatDataTable = DT_GameTable.Object;
    }

    static ConstructorHelpers::FObjectFinder<UDataTable> DT_EffectTable(TEXT("/Game/Data/Effect/DT_EffectTable.DT_EffectTable"));
    if (DT_EffectTable.Succeeded())
    {
        EffectTable = DT_EffectTable.Object;
    }
    
    if(!IsRunningDedicatedServer())
    {
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
        
        static ConstructorHelpers::FClassFinder<UUserWidget> SeamlessLoadingWidgetAsset(TEXT("UserWidget'/Game/Blueprints/HUD/BPW_SeamlessLoading.BPW_SeamlessLoading_C'"));
        if (SeamlessLoadingWidgetAsset.Succeeded())
            SeamlessLoadingWidgetClass = SeamlessLoadingWidgetAsset.Class;

        static ConstructorHelpers::FClassFinder<UUserWidget> LoadingWidgetAsset(TEXT("UserWidget'/Game/Blueprints/HUD/BPW_Loading.BPW_Loading_C'"));
        if (LoadingWidgetAsset.Succeeded())
            LoadingWidgetClass = LoadingWidgetAsset.Class;
        
        GetMoviePlayer()->OnPrepareLoadingScreen().AddUObject(this, &UEHGameInstance::ShowLoadingScreen);
    }
}

FStatStructure* UEHGameInstance::GetStatStructure(FName HeroName) const
{
    if (!CharacterStatDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Such DataTable"));
        return nullptr;
    }
    return CharacterStatDataTable->FindRow<FStatStructure>(HeroName, TEXT(""));
}

void UEHGameInstance::SetEffectArray()
{
    TArray<FEffectStructure*> AllEffects;
    EffectTable->GetAllRows(TEXT(""), AllEffects);
        
    for (FEffectStructure* Effect : AllEffects)
    {
        if (Effect)
        {
            AEffectBase::EffectArray.Add(Effect);
        }
    }
    UE_LOG(LogClass, Warning, TEXT("GameInstance : EffectArrayNums = %d"), AEffectBase::EffectArray.Num());
}

void UEHGameInstance::Init()
{
    Super::Init();
    if(!IsRunningDedicatedServer()) LoadSettings();
    else
    {
        SetEffectArray();
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
        }
    }
    else
    {
        // No settings file, use defaults
        SetDefaultSettings();
        SaveSettings();
    }

    ApplySettings();
}

void UEHGameInstance::SetRecommendedResolution()
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        FIntPoint NativeResolution = UserSettings->GetDesktopResolution();
        ResolutionWidth = NativeResolution.X;
        ResolutionHeight = NativeResolution.Y;
        UserSettings->SetScreenResolution(NativeResolution);
        UserSettings->ApplySettings(false);
    }
}

void UEHGameInstance::SetDefaultSettings()
{
    SetRecommendedResolution();
    ScreenMode = DefaultScreenMode;
    MaxFrame = DefaultMaxFrame;
    bVSyncEnabled = DefaultVSyncEnabled;
    OverallQuality = DefaultOverallQuality;
    AntiAliasing = DefaultAntiAliasing;
    PostProcessing = DefaultPostProcessing;
    MasterVolume = DefaultVolume;
    BackgroundVolume = DefaultVolume;
    SFXVolume = DefaultVolume;
    MouseSensitivity = DefaultMouseSensitivity;
}

void UEHGameInstance::ApplySettings()
{
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
        UserSettings->SetScreenResolution(FIntPoint(ResolutionWidth, ResolutionHeight));
        UserSettings->SetVSyncEnabled(bVSyncEnabled);
        UserSettings->SetOverallScalabilityLevel(OverallQuality);
        UserSettings->SetAntiAliasingQuality(AntiAliasing);
        UserSettings->SetPostProcessingQuality(PostProcessing);
        UserSettings->SetFrameRateLimit(MaxFrame);
        UserSettings->ApplySettings(false);
    }

    if (GEngine && GEngine->GetMainAudioDevice())
    {
        GEngine->GetMainAudioDevice()->SetSoundMixClassOverride(MainSoundMix, MasterVolumeSoundClass, MasterVolume, 1.0f, 0.0f, true);
        GEngine->GetMainAudioDevice()->SetSoundMixClassOverride(MainSoundMix, BackgroundVolumeSoundClass, BackgroundVolume, 1.0f, 0.0f, true);
        GEngine->GetMainAudioDevice()->SetSoundMixClassOverride(MainSoundMix, SFXVolumeSoundClass, SFXVolume, 1.0f, 0.0f, true);
        UGameplayStatics::PushSoundMixModifier(this, MainSoundMix);
    }
}


//seamless travel 때 띄울 로딩화면
void UEHGameInstance::ShowSeamlessLoadingScreen()
{
    if (SeamlessLoadingWidgetClass)
    {
        SeamlessLoadingWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), SeamlessLoadingWidgetClass));
        if (SeamlessLoadingWidget)
        {
            SeamlessLoadingWidget->AddToViewport();
            GetFirstLocalPlayerController()->bShowMouseCursor = false;
        }
    }
}

//seamless travel 때 띄운 로딩화면 제거
void UEHGameInstance::RemoveSeamlessLoadingScreen()
{
    if (SeamlessLoadingWidget) SeamlessLoadingWidget->RemoveFromParent();
}

//seamless가 아닌 이동마다 실행됨
void UEHGameInstance::ShowLoadingScreen()
{
    if (LoadingWidgetClass)
    {
        FLoadingScreenAttributes LoadingScreen;
        LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
        LoadingScreen.WidgetLoadingScreen = CreateWidget<UUserWidget>(this, LoadingWidgetClass)->TakeWidget();
        LoadingScreen.MinimumLoadingScreenDisplayTime = 1.5f; //최소 1.5초는 실행됨

        GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
        GetMoviePlayer()->PlayMovie();
    }
}