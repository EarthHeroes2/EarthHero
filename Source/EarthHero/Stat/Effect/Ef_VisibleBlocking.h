// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Stat/Effect/EffectBase.h"
#include "Ef_VisibleBlocking.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AEf_VisibleBlocking : public AEffectBase
{
	GENERATED_BODY()

	AEf_VisibleBlocking();

	UPROPERTY()
	UExponentialHeightFogComponent *ExponentialHeightFogComponent;
	
public:
	virtual void ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable, bool InbIsPermanent, bool InbShouldRefreshDuration) override;

	virtual void UpgradeEffect(float InEffectValue) override;
protected:
	virtual void ResetEffect() override;
	
	void SetFogSetting();
private:
	FTimerHandle VisibleBlockingHandle;
	void ResetVisibleBlockingClient();
	
};
