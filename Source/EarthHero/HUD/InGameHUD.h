// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"


UCLASS()
class EARTHHERO_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *ExpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *Level_Num;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *GameTimer_Tb;

	void InitializePlayerState(class UStatComponent *StatComponent);

	void UpdateGameTimer(int GameTimerSec);
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	class UStatComponent *StatComponentRef;
};
