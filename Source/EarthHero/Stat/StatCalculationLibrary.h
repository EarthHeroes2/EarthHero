#pragma once

#include "StatCalculationLibrary.generated.h"

struct FStatStructure;

UCLASS()
class EARTHHERO_API UStatCalculationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 받는 데미지
	UFUNCTION()
	static float CalNormalDamage(FStatStructure &HeroStat, float inDamage);

	UFUNCTION()
	static float CalWarriorNormalDamage(FStatStructure &HeroStat, float WR_NormalDamage);

	UFUNCTION()
	static float CalWarriorWheelWindDamage(FStatStructure &HeroStat, float WR_WheelWindDamage);
	
	UFUNCTION()
	static float CalWarriorDashDamage(FStatStructure &HeroStat, float WR_JumpDamageMulti, float WR_NormalDamage);

	UFUNCTION()
	static float CalShooterNormalDamage(FStatStructure &HeroStat, const FHitResult & HitInfo, float SH_HeadShot, float SH_NormalDamage);

	UFUNCTION()
	static float CalShooterGrenadeDamage(FStatStructure &HeroStat, float SH_GrenadeDamage);

	UFUNCTION()
	static bool AddExp(FStatStructure &HeroStat, FStatStructure &BaseHeroStat, int32 ExpMount);
};