#pragma once

#include "CoreMinimal.h"
#include "EarthHero/HUD/CoolDown.h"
#include "Status.generated.h"

USTRUCT(BlueprintType)
struct FStatus
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCoolDown *CoolDownWidget;

	FStatus()
		: EffectType(0)
		,CoolDownWidget(nullptr)
	{
	}

	FStatus(const int InEffectType, UCoolDown *InCoolDownWidget)
		: EffectType(InEffectType)
		, CoolDownWidget(InCoolDownWidget)
	{
	}

	// Equality operator for comparing EffectType
	bool operator==(const int InEffectType) const
	{
		return EffectType == InEffectType;
	}
};

USTRUCT(BlueprintType)
struct FEffectStatus
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EffectImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EffectType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectDuration;
};
