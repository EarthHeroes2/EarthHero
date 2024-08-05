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

	UFUNCTION()
	void IndexBtnClicked();
	
	

public:
	int Index;
	UPerkWidget* PerkWidget;
};
