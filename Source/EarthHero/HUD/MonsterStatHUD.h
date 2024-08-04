// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EarthHero/HUD/CoolDown.h"
#include "Structure/Status.h"
#include "MonsterStatHUD.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UMonsterStatHUD : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;
	
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MonsterLevel;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_1;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_2;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_3;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_4;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_5;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_6;
	UPROPERTY(meta = (BindWidget))
	UCoolDown *Status_7;


	UPROPERTY()
	TArray<FStatus> StatusArray;
	int EffectCount = 0;

	void UpdateHealth(float HeathPercent);
	
	void AddStatusImage(UTexture2D* EffectImage, int EffectType, float CoolDown);
	int CheckEffectTypeAlreadyExists(int EffectType);
	void DeleteStatusImage(int EffectType);
	
};
