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
	FString StatusName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCoolDown *CoolDownWidget;

	FStatus()
		: StatusName(TEXT("DefaultStatus"))
		,CoolDownWidget(nullptr)
	{
	}

	FStatus(const FString& InStatusName, UCoolDown *InCoolDownWidget)
		: StatusName(InStatusName)
		, CoolDownWidget(InCoolDownWidget)
	{
	}
};
