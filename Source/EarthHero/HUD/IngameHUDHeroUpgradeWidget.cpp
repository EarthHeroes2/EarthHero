#include "IngameHUDHeroUpgradeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UIngameHUDHeroUpgradeWidget::SetUpgradeNumber(const FText& Value)
{
	if (UpgradeNumber)
	{
		UpgradeNumber->SetText(Value);
	}
}

void UIngameHUDHeroUpgradeWidget::SetHeroUpgradeName(const FText& Value)
{
	if (HeroUpgradeName)
	{
		HeroUpgradeName->SetText(Value);
	}
}

void UIngameHUDHeroUpgradeWidget::SetHeroUpgradeDetail(const FText& Value)
{
	if (HeroUpgradeDetail)
	{
		HeroUpgradeDetail->SetText(Value);
	}
}

void UIngameHUDHeroUpgradeWidget::SetHeroUpgradeImg(UTexture2D* Texture)
{
	if (HeroUpgradeImg && Texture)
	{
		HeroUpgradeImg->SetBrushFromTexture(Texture);
	}
}

void UIngameHUDHeroUpgradeWidget::SetLevel1Img(UTexture2D* Texture)
{
	if (Level1Img && Texture)
	{
		Level1Img->SetBrushFromTexture(Texture);
	}
}

void UIngameHUDHeroUpgradeWidget::SetLevel2Img(UTexture2D* Texture)
{
	if (Level2Img && Texture)
	{
		Level2Img->SetBrushFromTexture(Texture);
	}
}

void UIngameHUDHeroUpgradeWidget::SetLevel3Img(UTexture2D* Texture)
{
	if (Level3Img && Texture)
	{
		Level3Img->SetBrushFromTexture(Texture);
	}
}
