// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkWidget.generated.h"

class UPerkButtonWidget;
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

	void CreateButtons();

	TSubclassOf<UPerkButtonWidget> PerkButtonClass;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Perk_Hb;
};
