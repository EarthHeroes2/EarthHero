// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "BasicStatsWidget.generated.h"

UCLASS()
class EARTHHERO_API UBasicStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillScore_Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalDamage_Text;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalShield_Text;
    
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalHeal_Text;
	
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetKillScore(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetTotalDamage(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetTotalShield(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetTotalHeal(FText Value);

	void HideAll();
};
