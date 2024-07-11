#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameHUDHeroUpgradeWidget.generated.h"

UCLASS()
class EARTHHERO_API UIngameHUDHeroUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetUpgradeNumber(const FText& Value);

	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetHeroUpgradeName(const FText& Value);

	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetHeroUpgradeDetail(const FText& Value);

	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetHeroUpgradeImg(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetLevel1Img(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetLevel2Img(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "HeroUpgrade")
	void SetLevel3Img(UTexture2D* Texture);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeNumber;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeroUpgradeName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeroUpgradeDetail;

	UPROPERTY(meta = (BindWidget))
	class UImage* HeroUpgradeImg;

	UPROPERTY(meta = (BindWidget))
	class UImage* Level1Img;

	UPROPERTY(meta = (BindWidget))
	class UImage* Level2Img;

	UPROPERTY(meta = (BindWidget))
	class UImage* Level3Img;
};
