// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Stat/Structure/StatStructure.h"
#include "EHGameInstance.generated.h"


UCLASS()
class EARTHHERO_API UEHGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

public:
	FStatStructure* GetStatStructure(FName HeroName) const;
	FString ServerPortNumber;

	int Difficulty = 1; //기본 값 1

	void ShowSeamlessLoadingScreen();
	void RemoveSeamlessLoadingScreen();

	void ShowLoadingScreen();

	TSubclassOf<class UUserWidget> SeamlessLoadingWidgetClass;
	UUserWidget* SeamlessLoadingWidget;

	TSubclassOf<class UUserWidget> LoadingWidgetClass;
	UUserWidget* LoadingWidget;
	
protected:
	UEHGameInstance();

private:
	UPROPERTY()
	class UDataTable* CharacterStatDataTable = nullptr;
	UPROPERTY()
	class UDataTable* EffectTable = nullptr;

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
	
};
