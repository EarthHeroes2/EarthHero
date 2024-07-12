// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "EarthHero/Enum/Enums.h"
#include "TeamMemberWidget.generated.h"

UCLASS()
class EARTHHERO_API UTeamMemberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ClassText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNumberText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExpText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	
	void SetName(const FText& Name);
	void SetClass(const EClassType ClassType);
	void SetPlayerNumber(const FText& PlayerNumber) const;
	void SetLevel(const int32 Level) const;
	void SetExp(const float ExpPercentage) const;
	void SetProgress(const float Progress) const;
};