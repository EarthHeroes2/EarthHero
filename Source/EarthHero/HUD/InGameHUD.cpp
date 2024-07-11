#include "InGameHUD.h"
#include "EarthHero/Stat/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "IngameHUDHeroUpgradeWidget.h"

void UInGameHUD::InitializePlayerState(UStatComponent* StatComponent)
{
	if (!StatComponent)
	{
		UE_LOG(LogClass, Warning, TEXT("InGameHUD: ERROR!! StatComponent is NULL"));
	}
	StatComponentRef = StatComponent;
}

void UInGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (StatComponentRef)
	{
		HealthBar->SetPercent(StatComponentRef->GetHealthPercent());
	}
}

void UInGameHUD::UpdateGameTimer(int GameTimerSec)
{
	GameTimer_Tb->SetText(FText::Format(FText::FromString("{0} : {1}"), FText::AsNumber(GameTimerSec / 60), FText::AsNumber(GameTimerSec % 60)));
}

//우선 세 개의 히어로 업그레이드 기준으로 만듦
void UInGameHUD::SetIngameHUDHeroUpgrade(int Index, UTexture2D* UpgradeImage, UTexture2D* Level1Image, UTexture2D* Level2Image, UTexture2D* Level3Image, FText UpgradeName, FText UpgradeDetail)
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
		TargetWidget->SetLevel1Img(Level1Image);
		TargetWidget->SetLevel2Img(Level2Image);
		TargetWidget->SetLevel3Img(Level3Image);
	}
}
