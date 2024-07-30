// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structure/Status.h"
#include "InGamePlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UInGamePlayerInfo : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerClass;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_1;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_2;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_3;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_4;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_5;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_6;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_7;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_8;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_9;
	UPROPERTY(meta = (BindWidget))
	class UCoolDown *Player_Status_10;

	TArray<FStatus> PLayerStatusArray;

	void SetName(const FString& Name) const;
	void SetProgress(const float Progress) const;
	void SetImage(UTexture2D *Image) const;
	void UpdatePlayerEffects(FEffectStatus PlayerStatus);
	void DeletePlayerEffects(int EffectType);

	UPROPERTY()
	int EffectCount = 0;

protected:
	virtual bool Initialize() override;
};
