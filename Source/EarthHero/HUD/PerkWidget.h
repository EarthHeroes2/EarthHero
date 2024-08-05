// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkWidget.generated.h"

class UButton;
class UIndexButton;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UPerkWidget : public UUserWidget
{
	GENERATED_BODY()

	UPerkWidget(const FObjectInitializer &ObjectInitializer);
	virtual void NativeConstruct() override;

	int NumOfLevels = 10;
	int NumOfPerkPerLevel = 5;

	TArray<UIndexButton*> Buttons;

	void CreateButtons();
	void PerkBtnClicked();
	void PerkSaveBtnClicked();
	void PerkCancelBtnClicked();

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Perk_Hb;

	UPROPERTY(meta = (BindWidget))
	UButton* PerkSave_Btn;

	UPROPERTY(meta = (BindWidget))
	UButton* PerkCancel_Btn;

	int64 SelectInfo;

public:
	void UpdateSelectInfo(int Index);
	
};
