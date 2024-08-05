// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Stat/Structure/HeroSkillImage.h"
#include "Stat/Structure/StatStructure.h"
#include "EHGameInstance.generated.h"


UCLASS()
class EARTHHERO_API UEHGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

public:
	FStatStructure* GetStatStructure(FName HeroName) const;
	FHeroSkillImage* GetSkillImageStructure(FName HeroName) const;
	FString ServerPortNumber;

	void SetEffectArray();
	int Difficulty = 1; //기본 값 1

	void ShowSeamlessLoadingScreen();
	void RemoveSeamlessLoadingScreen();

	void ShowLoadingScreen();
	int GetPlayerLevel();

	TSubclassOf<class UUserWidget> SeamlessLoadingWidgetClass;
	UUserWidget* SeamlessLoadingWidget;

	TSubclassOf<class UUserWidget> LoadingWidgetClass;
	UUserWidget* LoadingWidget;

	int64 PerkInfo;
	
protected:
	UEHGameInstance();

private:
	UPROPERTY()
	class UDataTable* CharacterStatDataTable = nullptr;
	UPROPERTY()
	class UDataTable* EffectTable = nullptr;
	UPROPERTY()
	class UDataTable* SkillImageTable = nullptr;

	void SetDefaultSettings();
	void SetRecommendedResolution();
	void ApplySettings();

public:
	bool bGameClear = true; //클리어 못할 시 false 적용해줘야함
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* MainSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* MasterVolumeSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* BackgroundVolumeSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* SFXVolumeSoundClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float BackgroundVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SFXVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ResolutionWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ResolutionHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 ScreenMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 MaxFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bVSyncEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 OverallQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 AntiAliasing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 PostProcessing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MouseSensitivity;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadSettings();




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

	
};
