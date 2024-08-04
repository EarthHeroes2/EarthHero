// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroUpgradeLibrary.h"

#include "ShooterStatComponent.h"
#include "WarriorStatComponent.h"
#include "EarthHero/Character/Warrior/EHWarrior.h"
#include "EarthHero/Character/Warrior/WarriorCombatComponent.h"

void UHeroUpgradeLibrary::Pb_NormalAttackDamage(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
                                                FStatStructure& HeroStat)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	
	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			BaseHeroStat.NormalDamage += 0.4f;
			HeroStat.NormalDamage = BaseHeroStat.NormalDamage;
			break;
		case 2 :
			BaseHeroStat.NormalDamage += 0.56f;
			HeroStat.NormalDamage = BaseHeroStat.NormalDamage;
			break;
		case 3 :
			BaseHeroStat.NormalDamage += 0.84f;
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
			BaseHeroStat.AttackSpeed += 0.3f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			break;
		case 2 :
			BaseHeroStat.AttackSpeed += 0.42f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			break;
		case 3 :
			BaseHeroStat.AttackSpeed += 0.63f;
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
			BaseHeroStat.SkillDamage += 0.75f;
			HeroStat.SkillDamage = BaseHeroStat.SkillDamage;
			break;
		case 2 :
			BaseHeroStat.SkillDamage += 1.05f;
			HeroStat.SkillDamage = BaseHeroStat.SkillDamage;
			break;
		case 3 :
			BaseHeroStat.SkillDamage += 1.58f;
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
			BaseHeroStat.SkillCoolTime -= 0.06f;
			HeroStat.SkillCoolTime = BaseHeroStat.SkillCoolTime;
			break;
		case 2 :
			BaseHeroStat.SkillCoolTime -= 0.09f;
			HeroStat.SkillCoolTime = BaseHeroStat.SkillCoolTime;
			break;
		case 3 :
			BaseHeroStat.SkillCoolTime -= 0.12f;
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
			BaseHeroStat.DashCoolTime -= 0.08f;
			HeroStat.DashCoolTime = BaseHeroStat.DashCoolTime;
			break;
		case 2 :
			BaseHeroStat.DashCoolTime -= 0.12f;
			HeroStat.DashCoolTime = BaseHeroStat.DashCoolTime;
			break;
		case 3 :
			BaseHeroStat.DashCoolTime -= 0.16f;
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
			BaseHeroStat.HealthUpgrade += 0.6f;
			BaseHeroStat.MaxHealth = 100 * BaseHeroStat.HealthUpgrade;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			break;
		case 2 :
			BaseHeroStat.HealthUpgrade += 0.84f;
			BaseHeroStat.MaxHealth = 100 * BaseHeroStat.HealthUpgrade;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			break;
		case 3 :
			BaseHeroStat.HealthUpgrade += 1.26f;
			BaseHeroStat.MaxHealth = 100 * BaseHeroStat.HealthUpgrade;
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
			BaseHeroStat.HealthRegeneration += 1.f;
			HeroStat.HealthRegeneration = BaseHeroStat.HealthRegeneration;
			break;
		case 2 :
			BaseHeroStat.HealthRegeneration += 1.4f;
			HeroStat.HealthRegeneration = BaseHeroStat.HealthRegeneration;
			break;
		case 3 :
			BaseHeroStat.HealthRegeneration += 2.1f;
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
			BaseHeroStat.MovementSpeed += 0.3f;
			HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;
			break;
		case 2 :
			BaseHeroStat.MovementSpeed += 0.42f;
			HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;
			break;
		case 3 :
			BaseHeroStat.MovementSpeed += 0.63f;
			HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;
			break ;
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::WR_Berserker(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UWarriorStatComponent* WarriorStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	WarriorStatComponent->HU_BerserkerLv += 1;
	
	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			//최대 체력 0.5
			BaseHeroStat.MaxHealth = 50 * BaseHeroStat.HealthUpgrade;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			if (HeroStat.Health > HeroStat.MaxHealth)
			{
				HeroStat.Health = HeroStat.MaxHealth;
			}

			//공격 속도 증가
			BaseHeroStat.AttackSpeed += 0.2f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;

			//흡혈 추가
			WarriorStatComponent->WR_Drain = 0.01f;
			break;
			
		case 2 :
			BaseHeroStat.AttackSpeed += 0.3f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			WarriorStatComponent->WR_Drain = 0.025f;
			break;
			
		case 3 :
			BaseHeroStat.AttackSpeed += 0.5f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;
			WarriorStatComponent->WR_Drain = 0.05f;
			break ;
			
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::WR_Guardian(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UWarriorStatComponent* WarriorStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	WarriorStatComponent->HU_GuardianLv += 1;
	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			//공격 속도 0.5
			BaseHeroStat.AttackSpeed *= 0.5f;
			HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;

			//최대 체력 증가
			BaseHeroStat.HealthUpgrade += 0.3f;
			BaseHeroStat.MaxHealth = 100 * BaseHeroStat.HealthUpgrade;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;

			//일반 공격 넉백 추가
			break;
				
		case 2 :
			//최대 체력 증가
			BaseHeroStat.MaxHealth += 0.35f;
			BaseHeroStat.MaxHealth = 100 * BaseHeroStat.HealthUpgrade;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			break;
				
		case 3 :
			BaseHeroStat.MaxHealth += 0.65f;
			BaseHeroStat.MaxHealth = 100 * BaseHeroStat.HealthUpgrade;
			HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
			//휠 윈드 스킬 넉백 추가
			break ;
				
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
}

void UHeroUpgradeLibrary::WR_JumpEnhanced(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UWarriorStatComponent* WarriorStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	WarriorStatComponent->HU_EnhnacedJumpLv += 1;

	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			WarriorStatComponent->WR_EnHancedJumpDistance = 1.25f;
			WarriorStatComponent->WR_EnHancedJumpSpeed = 1.25f;
			WarriorStatComponent->WR_JumpDamageMulti = 0.5f;
			break;
					
		case 2 :
			WarriorStatComponent->WR_EnHancedJumpDistance = 1.5f;
			WarriorStatComponent->WR_EnHancedJumpSpeed = 1.5f;
			WarriorStatComponent->WR_JumpDamageMulti = 1.f;
			break;
					
		case 3 :
			WarriorStatComponent->WR_EnHancedJumpDistance = 1.75f;
			WarriorStatComponent->WR_EnHancedJumpSpeed = 1.75f;
			WarriorStatComponent->WR_JumpDamageMulti = 2.f;
			break ;
					
		default:
			UE_LOG(LogClass, Error, TEXT("UHeroUpgradeLibrary : 히어로 업그레이드 레벨이 1, 2, 3 이 아닌 다른값이 들어옴"));
	}
	
}

void UHeroUpgradeLibrary::WR_WheelWind(FHeroUpgradeStructure& SelectHeroUpgrade, FStatStructure& BaseHeroStat,
	FStatStructure& HeroStat, UWarriorStatComponent* WarriorStatComponent)
{
	//선택한 히어로 업그레이드는 레벨 업
	SelectHeroUpgrade.UpgradeLevel += 1;
	WarriorStatComponent->HU_WheelWindLv += 1;
	switch (SelectHeroUpgrade.UpgradeLevel)
	{
		case 1 :
			WarriorStatComponent->WR_WheelWindDuration = 4.5f;
			WarriorStatComponent->Warrior->GetCombatComponent()->SetWheelWindDuration(4.5f);
			break;
						
		case 2 :
			//휠 윈드 사용 중 이동속도 1.5, 입는 피해 30% 감소
			break;
						
		case 3 :
			WarriorStatComponent->WR_WheelWindTick = 0.15f;
			WarriorStatComponent->Warrior->GetCombatComponent()->SetWheelWindTick(0.15f);
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
