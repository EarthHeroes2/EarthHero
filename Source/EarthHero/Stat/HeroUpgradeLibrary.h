// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Structure/HeroUpgrageStructure.h"
#include "Structure/StatStructure.h"
#include "HeroUpgradeLibrary.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UHeroUpgradeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//공용 히어로 업그리이드
	UFUNCTION()
	static void Pb_NormalAttackDamage(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);

	UFUNCTION()
	static void Pb_NormalAttackSpeed(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);
	
	UFUNCTION()
	static void Pb_AttackSkillDamage(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);

	UFUNCTION()
	static void Pb_AttackSkillCooldown(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);

	UFUNCTION()
	static void Pb_MovementSkillCooldown(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);
	
	UFUNCTION()
	static void Pb_MaxHealth(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);

	UFUNCTION()
	static void Pb_HealthRegenPerSecond(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);

	UFUNCTION()
	static void Pb_MovementSpeed(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat);

	//워리어 히어로 업그레이드

	//메카닉 히어로 업그레이드

	//슈터 히어로 업그레이드
	UFUNCTION()
	static void St_Headshot(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat, class UShooterStatComponent *ShooterStatComponent);

	UFUNCTION()
	static void St_IncreasedAmmoCapacity(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat, class UShooterStatComponent *ShooterStatComponent);

	UFUNCTION()
	static void St_GrenadeEnhancement(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat, class UShooterStatComponent *ShooterStatComponent);

	UFUNCTION()
	static void St_RocketBooster(FHeroUpgradeStructure &SelectHeroUpgrade, FStatStructure &BaseHeroStat, FStatStructure &HeroStat, class UShooterStatComponent *ShooterStatComponent);
	
};
