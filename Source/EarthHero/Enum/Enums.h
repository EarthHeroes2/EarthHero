// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

enum EClassType
{
	Warrior,
	Mechanic,
	Shooter,
	Archer,
	NumberOfClass
};

UENUM(BlueprintType)
enum class EHeroUpgrade : uint8
{
	Pb_NormalAttackDamage,
	Pb_NormalAttackSpeed,
	Pb_AttackSkillDamage,
	Pb_AttackSkillCooldown,
	Pb_MovementSkillCooldown,
	Pb_MaxHealth,
	Pb_HealthRegenPerSecond,
	Pb_MovementSpeed,
	Wr_Berserker,
	Wr_Guardian,
	Wr_Leap,
	Wr_Whirlwind,
	Mc_IncreasedDroneCount,
	Mc_VitalityDrone,
	Mc_ElectricShock,
	Mc_Teleport,
	St_Headshot,
	St_IncreasedAmmoCapacity,
	St_GrenadeEnhancement,
	St_RocketBooster,
	Ac_PoisonedArrows,
	Ac_CursedArrows,
	Ac_FanArrowsEnhancement,
	Ac_RollEnhancement
};
