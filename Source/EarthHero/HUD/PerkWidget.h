// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkWidget.generated.h"

class UEHGameInstance;
class UTextBlock;
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

	UEHGameInstance* EHGameInstance;

	int NumOfLevels = 10;
	int NumOfPerkPerLevel = 5;
	
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

	int64 SelectInfo;

public:
	void UpdateSelectInfo(int Index);
};
