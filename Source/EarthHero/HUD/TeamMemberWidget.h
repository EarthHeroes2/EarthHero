// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
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
	UTextBlock* LevelAndExpText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UFUNCTION(BlueprintCallable, Category = "TeamMember")
	void SetName(FText Name);

	UFUNCTION(BlueprintCallable, Category = "TeamMember")
	void SetClass(FText ClassName);

	UFUNCTION(BlueprintCallable, Category = "TeamMember")
	void SetPlayerNumber(FText PlayerNumber);

	UFUNCTION(BlueprintCallable, Category = "TeamMember")
	void SetLevelAndExp(int32 Level, float ExpPercentage);

	UFUNCTION(BlueprintCallable, Category = "TeamMember")
	void SetProgress(float Progress);
};