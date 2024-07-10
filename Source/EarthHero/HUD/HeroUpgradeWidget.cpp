#include "HeroUpgradeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UHeroUpgradeWidget::SetUpgradeImage(UTexture2D* Texture)
{
	if (UpgradeImage && Texture)
	{
		UpgradeImage->SetBrushFromTexture(Texture);
	}
}

void UHeroUpgradeWidget::SetLevel1Image(UTexture2D* Texture)
{
	if (Level1Image && Texture)
	{
		Level1Image->SetBrushFromTexture(Texture);
	}
}

void UHeroUpgradeWidget::SetLevel2Image(UTexture2D* Texture)
{
	if (Level2Image && Texture)
	{
		Level2Image->SetBrushFromTexture(Texture);
	}
}

void UHeroUpgradeWidget::SetLevel3Image(UTexture2D* Texture)
{
	if (Level3Image && Texture)
	{
		Level3Image->SetBrushFromTexture(Texture);
	}
}

void UHeroUpgradeWidget::SetUpgradeName(FText Value)
{
	if (UpgradeName)
	{
		UpgradeName->SetText(Value);
	}
}

void UHeroUpgradeWidget::SetUpgradeDesc(FText Value)
{
	if (UpgradeDesc)
	{
		UpgradeDesc->SetText(Value);
	}
}
