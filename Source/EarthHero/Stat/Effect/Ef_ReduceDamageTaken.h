// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBase.h"
#include "Ef_ReduceDamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AEf_ReduceDamageTaken : public AEffectBase
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable, bool InbIsPermanent, bool InbShouldRefreshDuration) override;

	virtual void UpgradeEffect(float InEffectValue) override;

protected:
	virtual void ResetEffect() override;
	
};
