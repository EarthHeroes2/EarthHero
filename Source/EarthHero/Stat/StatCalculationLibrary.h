#pragma once

#include "StatCalculationLibrary.generated.h"

struct FStatStructure;

UCLASS()
class EARTHHERO_API UStatCalculationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static float CalNormalDamage(FStatStructure &HeroStat, float inDamage);

	UFUNCTION()
	static float CalShooterNormalDamage(FStatStructure &HeroStat, const FHitResult & HitInfo, float SH_HeadShot, float SH_NormalDamage);

	UFUNCTION()
	static float CalShooterGrenadeDamage(FStatStructure &HeroStat, float SH_GrenadeDamage);

	UFUNCTION()
	static bool AddExp(FStatStructure &HeroStat, FStatStructure &BaseHeroStat, int32 ExpMount);
};