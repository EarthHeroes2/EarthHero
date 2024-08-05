// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "IndexButton.generated.h"

class UPerkWidget;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UIndexButton : public UButton
{
	GENERATED_BODY()

	UIndexButton();

	bool bSelected = false;

	int NeedPoint[50] = {1, 1, 1, 2, 2,
						 1, 1, 2, 2, 3,
						 1, 2, 3, 3, 4,
						 1, 1, 2, 2, 2,
						 1, 2, 3, 4, 5,
						 2, 2, 2, 4, 4,
						 1, 2, 2, 4, 5,
						 1, 2, 2, 3, 3,
						 2, 2, 2, 3, 5,
						 1, 1, 3, 5, 5};

public:
	int Index;
	UPerkWidget* PerkWidget;

	UFUNCTION()
	void IndexBtnClicked();
};
