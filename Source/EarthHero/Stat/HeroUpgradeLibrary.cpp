// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroUpgradeLibrary.h"

#include "ShooterStatComponent.h"

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

void UHeroUpgradeLibrary::St_Headshot(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UShooterStatComponent* ShooterStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	ShooterStatComponent->HU_HeadShotLv += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			ShooterStatComponent->SH_HeadShot = 1.3f;
			break;
		case 2 :
			ShooterStatComponent->SH_HeadShot = 1.75f;
			break;
		case 3 :
			ShooterStatComponent->SH_HeadShot = 2.5f;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::St_IncreasedAmmoCapacity(FHeroUpgradeStructure& SelectHeroUpgrade,
	FStatStructure& BaseHeroStat, FStatStructure& HeroStat, UShooterStatComponent* ShooterStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	ShooterStatComponent->HU_ClipsLv += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			ShooterStatComponent->SH_Clips = 70;
			break;
		case 2 :
			ShooterStatComponent->SH_Clips = 150;
			break;
		case 3 :
			ShooterStatComponent->SH_ClipsEternal = true;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::St_GrenadeEnhancement(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UShooterStatComponent* ShooterStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	ShooterStatComponent->HU_EnhnacedGenerate += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			ShooterStatComponent->SH_GrenadeRange *= 2;
			break;
		case 2 :
			ShooterStatComponent->SH_AdditionalDamage = 0.3f;
			break;
		case 3 :
			//적 이동 잠시 속박되고, 폭발 지점 중심으로 적들이 소폭 끌어당겨짐.
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::St_RocketBooster(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UShooterStatComponent* ShooterStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	ShooterStatComponent->HU_EnhancedBooster += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			ShooterStatComponent->SH_FlightTime = 3.5f;
			ShooterStatComponent->SH_FLightSpeed = 1.7f;
			ShooterStatComponent->SH_FlightAttackSpeed = 0.55f;
			break;
		case 2 :
			ShooterStatComponent->SH_FlightTime = 3.f;
			ShooterStatComponent->SH_FLightSpeed = 2.f;
			ShooterStatComponent->SH_FlightAttackSpeed = 1.11f;
			break;
		case 3 :
			ShooterStatComponent->SH_FlightTime = 2.5f;
			ShooterStatComponent->SH_FLightSpeed = 2.5f;
			ShooterStatComponent->SH_FlightAttackSpeed = 2.22f;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}
