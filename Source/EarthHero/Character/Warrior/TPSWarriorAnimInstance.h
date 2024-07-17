// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EarthHero/Character/EHAnimInstanceBase.h"
#include "TPSWarriorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UTPSWarriorAnimInstance : public UEHAnimInstanceBase
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UFUNCTION()
	void AnimNotify_SwordHit();
	
};
