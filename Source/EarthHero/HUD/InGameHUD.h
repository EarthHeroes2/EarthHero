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

	void InitializePlayerState(class UStatComponent *StatComponent);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	class UStatComponent *StatComponentRef;
};
