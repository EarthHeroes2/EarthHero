#pragma once

#include "StatCalculationLibrary.generated.h"

struct FStatStructure;

UCLASS()
class EARTHHERO_API UStatCalculationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static void CalNormalDamage(FStatStructure &HeroStat, float inDamage);

	UFUNCTION()
	static float CalShooterNormalDamage(FStatStructure &HeroStat, const FHitResult & HitInfo, int32 SH_HeadShot);

	UFUNCTION()
	static float CalShooterGrenadeDamage(FStatStructure &HeroStat, float SH_GrenadeDamage);

	UFUNCTION()
	static bool AddExp(FStatStructure &HeroStat, FStatStructure &BaseHeroStat, int32 ExpMount);
};