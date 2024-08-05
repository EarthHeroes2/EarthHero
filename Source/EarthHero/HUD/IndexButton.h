// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "IndexButton.generated.h"

class UEHGameInstance;
class UPerkWidget;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UIndexButton : public UButton
{
	GENERATED_BODY()

	UIndexButton();

	UEHGameInstance* EHGameInstance;
	int NeedPoint;
	
	bool bSelected = false;

public:
	int Index;
	UPerkWidget* PerkWidget;

	UFUNCTION()
	void IndexBtnClicked();
};
