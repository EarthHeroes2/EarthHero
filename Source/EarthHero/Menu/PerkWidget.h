#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkWidget.generated.h"

class UImage;
class PerkInfomation;
class UEHGameInstance;
class UTextBlock;
class UButton;
class UIndexButton;
class UHorizontalBox;

UCLASS()
class EARTHHERO_API UPerkWidget : public UUserWidget
{
	GENERATED_BODY()

	UPerkWidget(const FObjectInitializer &ObjectInitializer);
	virtual bool Initialize() override;

	UEHGameInstance* EHGameInstance;

	PerkInfomation* PerkInfomations;
	
	int Level;
public:
	int Point;
private:
	TArray<UIndexButton*> Buttons;

	void CreateButtons();

	UFUNCTION()
	void PerkSaveBtnClicked();
	UFUNCTION()
	void PerkCancelBtnClicked();

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Perk_Hb;

	UPROPERTY(meta = (BindWidget))
	UButton* PerkSave_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PerkCancel_Btn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Level_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Point_Tb;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PerkName_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PerkCost_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PerkDescription_Tb;
	UPROPERTY(meta = (BindWidget))
	UImage* PerkImage_Img;

	int64 SelectInfo;

	UTextBlock* Description_Tb;

public:
	void UpdateSelectInfo(int Index);
	void PerkButtonHovered(int Index);
	void PerkButtonUnhovered(int Index);
};
