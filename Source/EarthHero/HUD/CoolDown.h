// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoolDown.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UCoolDown : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage *EffectImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *Text_RemainingTime;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar *ProgressBar_CoolDown;

	UPROPERTY()
	class UInGameHUD *InGameHUD;

	UPROPERTY()
	class UInGamePlayerInfo *InGamePlayerInfo;
	
	void SetImage(UTexture2D *Image);
	void ClearImage();
	void StartCoolDown(float CoolDown, int InEffectType);

	FTimerHandle ProgressHandle;
	FTimerHandle EffectEndHandle;

	void Loop();
	void End();
	UPROPERTY()
	int EffectType;
	UPROPERTY()
	float SubtractPercent;
	UPROPERTY()
	float RemainingTime;
	UPROPERTY()
	float Percent;
};
