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

	UEHGameInstance* EHGameInstance;
	int NeedPoint = -1;
	
	bool bSelected = false;

public:
	int Index;
	UPerkWidget* PerkWidget;

	void InitSetting(int ReceivedIndex, UPerkWidget* ParentWidget);
	UFUNCTION()
	void IndexBtnClicked();
};
