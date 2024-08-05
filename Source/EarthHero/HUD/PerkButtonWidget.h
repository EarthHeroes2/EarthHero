// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerkButtonWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UPerkButtonWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void PerkBtnClicked();

public:
	int Index;

	UPROPERTY(meta = (BindWidget))
	UButton* Perk_Btn;
	void AddOnClick();
};
