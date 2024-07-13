#include "InGameHUD.h"
#include "EarthHero/Stat/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "IngameHUDHeroUpgradeWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "EarthHero/Player/EHPlayerController.h"

bool UInGameHUD::Initialize()
{
	Super::Initialize();

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
}

void UInGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (StatComponentRef)
	{
		HealthBar->SetPercent(StatComponentRef->GetHealthPercent());
		CurrentHealthText->SetText(FText::AsNumber(StatComponentRef->GetHealth()));
		MaxHealthText->SetText(FText::Format(FText::FromString(TEXT("/ {0}")), FText::AsNumber(StatComponentRef->GetMaxHealth())));
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