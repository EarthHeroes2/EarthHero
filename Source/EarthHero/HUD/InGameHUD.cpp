#include "InGameHUD.h"
#include "EarthHero/Stat/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "IngameHUDHeroUpgradeWidget.h"
#include "InGamePlayerInfo.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "TimerManager.h"
#include "Components/TextBlock.h"
#include "EarthHero/Enum/Enums.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "Kismet/GameplayStatics.h"

bool UInGameHUD::Initialize()
{
	Super::Initialize();

	InGamePlayerInfoArray.Add(BP_InGamePlayerInfo);
	InGamePlayerInfoArray.Add(BP_InGamePlayerInfo_1);
	InGamePlayerInfoArray.Add(BP_InGamePlayerInfo_2);
	InGamePlayerInfoArray.Add(BP_InGamePlayerInfo_3);

	StatusArray.Add(FStatus(0, Status));
	StatusArray.Add(FStatus(0, Status_1));
	StatusArray.Add(FStatus(0, Status_2));
	StatusArray.Add(FStatus(0, Status_3));
	StatusArray.Add(FStatus(0, Status_4));
	StatusArray.Add(FStatus(0, Status_5));
	StatusArray.Add(FStatus(0, Status_6));
	StatusArray.Add(FStatus(0, Status_7));
	StatusArray.Add(FStatus(0, Status_8));
	StatusArray.Add(FStatus(0, Status_9));
	Chat_Etb->OnTextCommitted.AddDynamic(this, &ThisClass::ChatTextCommitted);
	
	if (HeroUpgradeVerticalBox)
	{
		HeroUpgradeVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	return true;
}

void UInGameHUD::InitializePlayerState(UStatComponent* StatComponent)
{
	if (!StatComponent)
	{
		UE_LOG(LogClass, Warning, TEXT("InGameHUD: ERROR!! StatComponent is NULL"));
	}
	StatComponentRef = StatComponent;

	UEHGameInstance* ABGameInstance = Cast<UEHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == ABGameInstance)
	{
		return ;
	}
	FHeroSkillImage *HeroSkillImage;
	switch (HeroType)
	{
		case Warrior:
			HeroSkillImage = ABGameInstance->GetSkillImageStructure("Warrior");
			SkillCoolDown->SetImage(HeroSkillImage->NormalAttackImage);
			SkillCoolDown_1->SetImage(HeroSkillImage->SkillImage);
			SkillCoolDown_2->SetImage(HeroSkillImage->DashImage);
			break;
		case Mechanic:
			HeroSkillImage = ABGameInstance->GetSkillImageStructure("Mechanic");
			SkillCoolDown->SetImage(HeroSkillImage->NormalAttackImage);
			SkillCoolDown_1->SetImage(HeroSkillImage->SkillImage);
			SkillCoolDown_2->SetImage(HeroSkillImage->DashImage);
			break;
		case Shooter:
			HeroSkillImage = ABGameInstance->GetSkillImageStructure("Shooter");
			SkillCoolDown->SetImage(HeroSkillImage->NormalAttackImage);
			SkillCoolDown_1->SetImage(HeroSkillImage->SkillImage);
			SkillCoolDown_2->SetImage(HeroSkillImage->DashImage);
			break;
		case Archer:
			HeroSkillImage = ABGameInstance->GetSkillImageStructure("Archer");
			SkillCoolDown->SetImage(HeroSkillImage->NormalAttackImage);
			SkillCoolDown_1->SetImage(HeroSkillImage->SkillImage);
			SkillCoolDown_2->SetImage(HeroSkillImage->DashImage);
			break;
	}
}

void UInGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (StatComponentRef)
	{
		HealthBar->SetPercent(StatComponentRef->GetHealthPercent());
		CurrentHealthText->SetText(FText::AsNumber(FMath::FloorToInt(StatComponentRef->GetHealth())));
		MaxHealthText->SetText(FText::Format(FText::FromString(TEXT("/ {0}")), FText::AsNumber(FMath::FloorToInt(StatComponentRef->GetMaxHealth()))));
	}
}

void UInGameHUD::UpdateGameTimer(int GameTimerSec)
{
	int32 Minutes = GameTimerSec / 60;
	int32 Seconds = GameTimerSec % 60;

	FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	GameTimer_Tb->SetText(FText::FromString(TimeString));
}

void UInGameHUD::UpdatePlayerInfoVisibility(int32 AllPlayerNumbers)
{
	for (int32 i = 0; i < InGamePlayerInfoArray.Num(); ++i)
	{
		if (i < AllPlayerNumbers)
		{
			InGamePlayerInfoArray[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InGamePlayerInfoArray[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

//우선 세 개의 히어로 업그레이드 기준으로 만듦
void UInGameHUD::SetIngameHUDHeroUpgrade(int Index, UTexture2D* UpgradeImage, int Level, FText UpgradeName, FText UpgradeDetail)
{
	UIngameHUDHeroUpgradeWidget* TargetWidget = nullptr;

	switch (Index)
	{
	case 0:
		TargetWidget = BP_IngameHUDHeroUpgrade;
		break;
	case 1:
		TargetWidget = BP_IngameHUDHeroUpgrade_1;
		break;
	case 2:
		TargetWidget = BP_IngameHUDHeroUpgrade_2;
		break;
	default:
		UE_LOG(LogClass, Warning, TEXT("InGameHUD: Invalid Index for HeroUpgradeWidget"));
		return;
	}

	if (TargetWidget)
	{
		FText UpgradeNumberText = FText::AsNumber(Index + 1); //0번째 인덱스 -> 증강에선 1로 나옴
		TargetWidget->SetUpgradeNumber(UpgradeNumberText);
		TargetWidget->SetHeroUpgradeName(UpgradeName);
		TargetWidget->SetHeroUpgradeDetail(UpgradeDetail);
		TargetWidget->SetHeroUpgradeImg(UpgradeImage);
		
		UTexture2D* GoldStar = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/Full_Star_Yellow.Full_Star_Yellow'")));
		UTexture2D* WhiteStar = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/Empty_Star.Empty_Star'")));
		
		switch(Level)
		{
		case 0:
			TargetWidget->SetLevel1Img(GoldStar);
			TargetWidget->SetLevel2Img(WhiteStar);
			TargetWidget->SetLevel3Img(WhiteStar);
			break;
		case 1:
			TargetWidget->SetLevel1Img(GoldStar);
			TargetWidget->SetLevel2Img(GoldStar);
			TargetWidget->SetLevel3Img(WhiteStar);
			break;
		case 2:
			TargetWidget->SetLevel1Img(GoldStar);
			TargetWidget->SetLevel2Img(GoldStar);
			TargetWidget->SetLevel3Img(GoldStar);
			break;
		default:
			UE_LOG(LogClass, Warning, TEXT("InGameHUD: Invalid Level"));
			break;
		}
	}

	IsHeroUpGradeReady[Index] = true;
	ShowHeroUpgradeVerticalBox();
}

bool UInGameHUD::IsHeroUpgradeReadys() const
{
	if (IsHeroUpGradeReady[0] && IsHeroUpGradeReady[1] && IsHeroUpGradeReady[2])
		return true;
	return false;
}

void UInGameHUD::SetGameMessageText(FString& Message)
{
	if (GameMessage)
	{
		GameMessage->SetText(FText::FromString(Message));
		GameMessage->SetVisibility(ESlateVisibility::Visible);

		if (!GetWorld()->GetTimerManager().IsTimerActive(HideMessageTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(HideMessageTimerHandle, this, &UInGameHUD::HideGameMessage, 5.0f, false);
		}
	}
}

void UInGameHUD::HideGameMessage()
{
	if (GameMessage)
	{
		GameMessage->SetVisibility(ESlateVisibility::Collapsed);

		GetWorld()->GetTimerManager().ClearTimer(HideMessageTimerHandle);
	}
}

void UInGameHUD::SetFalseHeroUpgradeReady()
{
	IsHeroUpGradeReady[0] = false;
	IsHeroUpGradeReady[1] = false;
	IsHeroUpGradeReady[2] = false;
	HideHeroUpgradeVerticalBox();
}

void UInGameHUD::ShowHeroUpgradeVerticalBox()
{
	if (HeroUpgradeVerticalBox)
	{
		HeroUpgradeVerticalBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInGameHUD::HideHeroUpgradeVerticalBox()
{
	if (HeroUpgradeVerticalBox)
	{
		HeroUpgradeVerticalBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameHUD::ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	FInputModeGameOnly InputMode;
	switch (CommitMethod)
	{
	case ETextCommit::Default:
	case ETextCommit::OnEnter:
		if (!Text.IsEmpty())
		{
			APlayerController* PlayerController = GetOwningPlayer();
			if (PlayerController)
			{
				AEHPlayerController* EHPlayerController= Cast<AEHPlayerController>(PlayerController);
				if (EHPlayerController)
					EHPlayerController->Server_SendChatMessage(Text);
			}
			Chat_Etb->SetText(FText::GetEmpty());
		}
		GetOwningPlayer()->SetInputMode(InputMode); //포커스 초기화
		break;
	case ETextCommit::OnUserMovedFocus:
	case ETextCommit::OnCleared:
		break;
	}
}

void UInGameHUD::AddChatMessage(const FText& Text)
{
	UTextBlock* TextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

	if (TextBlock)
	{
		FSlateFontInfo FontInfo = TextBlock->GetFont();
		FontInfo.Size = 16;
		
		TextBlock->SetFont(FontInfo);
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

void UInGameHUD::UpdatePlayerEffectState(const FEffectStatus EffectStatus, const int32 TargetIndex)
{
	//UE_LOG(LogClass, Warning, TEXT("InGameHUD : GameStateEffectStatuses"));
	InGamePlayerInfoArray[TargetIndex]->UpdatePlayerEffects(EffectStatus);
}

void UInGameHUD::AddStatusImage(UTexture2D* EffectImage, int EffectType, float CoolDown)
{
	int Index = 0;
	if ((Index = CheckEffectTypeAlreadyExists(EffectType)) == -1)
	{
		EffectCount += 1;
		//UE_LOG(LogClass, Warning, TEXT("EffectType = %d, EffectCount = %d"), EffectType, EffectCount);
		StatusArray[EffectCount - 1] = FStatus(EffectType, StatusArray[EffectCount - 1].CoolDownWidget);
		StatusArray[EffectCount - 1].CoolDownWidget->SetImage(EffectImage);
		StatusArray[EffectCount - 1].CoolDownWidget->StartCoolDown(CoolDown, EffectType);
		StatusArray[EffectCount - 1].CoolDownWidget->InGameHUD = this;
	}
	else
	{
		StatusArray[Index].CoolDownWidget->UpdateCoolDown(CoolDown);
	}
}

int UInGameHUD::CheckEffectTypeAlreadyExists(int EffectType)
{
	for (int index = 0; index < EffectCount; index++)
	{
		if (StatusArray[index].EffectType == EffectType)
		{
			return index;
		}
	}
	return -1;
}

void UInGameHUD::DeleteStatusImage(int EffectType)
{
	EffectCount -= 1;
	FStatus *FindStatus = Algo::FindByPredicate(StatusArray, [EffectType](const FStatus& Status)
	{
		return Status.EffectType == EffectType;
	});
	FindStatus->CoolDownWidget->ClearImage();
}

void UInGameHUD::UpdatePlayerHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths)
{
	int i;
	for (i = 0; i < PlayerMaxHealths.Num(); i++)
	{
		InGamePlayerInfoArray[i]->SetProgress(PlayerCurrentHealths[i] / PlayerMaxHealths[i]);
	}
	for (int j = i; j <= 3; j++)
	{
		InGamePlayerInfoArray[j]->SetVisibility(ESlateVisibility::Collapsed);
		for (FStatus PlayerStatus : InGamePlayerInfoArray[j]->PLayerStatusArray)
		{
			PlayerStatus.CoolDownWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInGameHUD::UpdatePlayerClassImages(const TArray<UTexture2D*> PlayerClassImages)
{
	for(int i = 0; i < PlayerClassImages.Num(); i++)
	{
		InGamePlayerInfoArray[i]->SetImage(PlayerClassImages[i]);
	}
}

void UInGameHUD::UpdatePlayerNames(const TArray<FString> AllPlayerNames)
{
	for(int i = 0; i < AllPlayerNames.Num(); i++)
	{
		InGamePlayerInfoArray[i]->SetName(AllPlayerNames[i]);
	}
}
