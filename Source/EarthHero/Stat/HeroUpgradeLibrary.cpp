// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroUpgradeLibrary.h"

void UHeroUpgradeLibrary::Pb_NormalAttackDamage(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	
	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.NormalDamage = 1.4f;
			HeroStat.NormalDamage = BaseHeroStat.NormalDamage;
			break;
		case 2 :
			BaseHeroStat.NormalDamage = 1.96f;
			HeroStat.NormalDamage = BaseHeroStat.NormalDamage;
			break;
		case 3 :
			BaseHeroStat.NormalDamage = 2.7f;
			HeroStat.NormalDamage = BaseHeroStat.NormalDamage;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_NormalAttackSpeed(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.AttackSpeed = 1.3f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			break;
		case 2 :
			BaseHeroStat.AttackSpeed = 1.72f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			break;
		case 3 :
			BaseHeroStat.AttackSpeed = 2.35f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_AttackSkillDamage(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.SkillDamage = 1.75f;
			HeroStat.SkillDamage = BaseHeroStat.SkillDamage;
			break;
		case 2 :
			BaseHeroStat.SkillDamage = 2.8f;
			HeroStat.SkillDamage = BaseHeroStat.SkillDamage;
			break;
		case 3 :
			BaseHeroStat.SkillDamage = 4.38f;
			HeroStat.SkillDamage = BaseHeroStat.SkillDamage;	
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_AttackSkillCooldown(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.SkillCoolTime = 0.94f;
			HeroStat.SkillCoolTime = BaseHeroStat.SkillCoolTime;
			break;
		case 2 :
			BaseHeroStat.SkillCoolTime = 0.85f;
			HeroStat.SkillCoolTime = BaseHeroStat.SkillCoolTime;
			break;
		case 3 :
			BaseHeroStat.SkillCoolTime = 0.73f;
			HeroStat.SkillCoolTime = BaseHeroStat.SkillCoolTime;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_MovementSkillCooldown(FHeroUpgradeStructure& SelectHeroUpgrade,
	FStatStructure& BaseHeroStat, FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
    	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.DashCoolTime = 0.92f;
			HeroStat.DashCoolTime = BaseHeroStat.DashCoolTime;
			break;
		case 2 :
			BaseHeroStat.DashCoolTime = 0.8f;
			HeroStat.DashCoolTime = BaseHeroStat.DashCoolTime;
			break;
		case 3 :
			BaseHeroStat.DashCoolTime = 0.64f;
			HeroStat.DashCoolTime = BaseHeroStat.DashCoolTime;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_MaxHealth(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.MaxHealth = 160;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			break;
		case 2 :
			BaseHeroStat.MaxHealth = 244;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			break;
		case 3 :
			BaseHeroStat.MaxHealth = 370;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_HealthRegenPerSecond(FHeroUpgradeStructure& SelectHeroUpgrade,
	FStatStructure& BaseHeroStat, FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.HealthRegeneration = 2.f;
			HeroStat.HealthRegeneration = BaseHeroStat.HealthRegeneration;
			break;
		case 2 :
			BaseHeroStat.HealthRegeneration = 3.4f;
			HeroStat.HealthRegeneration = BaseHeroStat.HealthRegeneration;
			break;
		case 3 :
			BaseHeroStat.HealthRegeneration = 4.5f;
			HeroStat.HealthRegeneration = BaseHeroStat.HealthRegeneration;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::Pb_MovementSpeed(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.MovementSpeed = 1.3f;
			HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;
			break;
		case 2 :
			BaseHeroStat.MovementSpeed = 1.72f;
			HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;
			break;
		case 3 :
			BaseHeroStat.MovementSpeed = 2.35f;
			HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}
