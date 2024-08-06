// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum EClassType
{
	Warrior,
	Mechanic,
	Shooter,
	Archer,
	NumberOfClass
};

UENUM(BlueprintType)
enum EHeroUpgrade
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
	Wr_Whirlwind,
	Wr_Leap,
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

//몬스터 타입
UENUM(BlueprintType)
enum MonsterType
{
	Normal,
	Boss
};

//Effect 타입
UENUM(BlueprintType)
enum EffectType
{
	Ef_Bind,
	Ef_IncreaseDamageTaken,
	Ef_PointImpulse,
	Ef_ReduceDamageTaken,
	Ef_SpeedBoost,
};

enum EBossName
{
	MainBoss,
	MidBoss1,
	MidBoss2,
	MidBoss3,
	MidBoss4,
	MidBoss5,
	MidBoss6,
	Max_Boss,
	NotBoss,
};

enum ERequest
{
	Client_CreateLobby, Client_ComparePassword, Client_GetPlayerData, Client_GetPlayerLevel, Client_Request_Max,
	Server_DestroyServer, Server_UpdatePlayerExp, Server_UpdatePassword, Server_GetPlayerLevel, Server_Request_Max
};