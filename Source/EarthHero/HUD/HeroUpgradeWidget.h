#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroUpgradeWidget.generated.h"

UCLASS()
class EARTHHERO_API UHeroUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUpgradeImage(UTexture2D* Texture);
	void SetLevel1Image(UTexture2D* Texture);
	void SetLevel2Image(UTexture2D* Texture);
	void SetLevel3Image(UTexture2D* Texture);
	void SetUpgradeName(FText Value);
	void SetUpgradeDesc(FText Value);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* UpgradeImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* Level1Image;

	UPROPERTY(meta = (BindWidget))
	class UImage* Level2Image;

	UPROPERTY(meta = (BindWidget))
	class UImage* Level3Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UpgradeDesc;
};
