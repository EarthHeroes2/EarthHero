

#include "StatCalculationLibrary.h"

#include "Structure/StatStructure.h"

float UStatCalculationLibrary::	CalNormalDamage(FStatStructure& HeroStat, float inDamage)
{
	//데미지 0이하 일 시 무시
	if (inDamage <= 0)
		return 0.f;

	// 남는 방어력이 있으면,
	if (HeroStat.DefensePower > 0)
	{
		float leftDamage = HeroStat.DefensePower - inDamage; //반대?

		//방어력을 넘는 데미지가 들어올 시
		if (leftDamage > 0)
		{
			HeroStat.DefensePower = 0;
			//실질 데미지 = 체력 - 데미지 
			float RealDamage = HeroStat.Health - leftDamage;
			if (RealDamage > 0)
			{
				HeroStat.Health -= leftDamage;
				return leftDamage;
			}
			else
			{
				HeroStat.Health = 0;
				return RealDamage;
			}
		}
		else
		{
			HeroStat.DefensePower -= inDamage;
			return inDamage;
		}
	}
	else
	{
		//실질 데미지 = 체력 - 데미지 
		float RealDamage = HeroStat.Health - inDamage;
		if (RealDamage > 0)
		{
			HeroStat.Health -= inDamage;
			return inDamage;
		}
		else
		{
			HeroStat.Health = 0;
			return RealDamage; //음수 리턴
		}
	}
}

float UStatCalculationLibrary::CalWarriorNormalDamage(FStatStructure& HeroStat, float WR_NormalDamage)
{
	return WR_NormalDamage * HeroStat.NormalDamage;
}

float UStatCalculationLibrary::CalWarriorWheelWindDamage(FStatStructure& HeroStat, float WR_WheelWindDamage)
{
	return WR_WheelWindDamage * HeroStat.SkillDamage;
}


float UStatCalculationLibrary::CalShooterNormalDamage(FStatStructure &HeroStat, const FHitResult & HitInfo, float SH_HeadShot, float SH_NormalDamage)
{
	float resultDamage = 0.f;
	if (HitInfo.BoneName == "head")
	{
		//UE_LOG(LogClass, Warning, TEXT("HeadShot, SH_HeadShot = %f"), SH_HeadShot);
		resultDamage = SH_NormalDamage * HeroStat.NormalDamage * SH_HeadShot;
	}
	else
	{
		resultDamage = SH_NormalDamage * HeroStat.NormalDamage;
	}
	return resultDamage;
}

float UStatCalculationLibrary::CalShooterGrenadeDamage(FStatStructure& HeroStat, float SH_GrenadeDamage)
{
	float resultDamage = 0.f;
	resultDamage = SH_GrenadeDamage * HeroStat.SkillDamage;
	
	return resultDamage;
}

bool UStatCalculationLibrary::AddExp(FStatStructure& HeroStat, FStatStructure &BaseHeroStat, int32 ExpMount)
{
	float resultExp = HeroStat.Exp + ExpMount;
	if (resultExp >= HeroStat.MaxExp)
	{
		while (resultExp >= HeroStat.MaxExp)
		{
			resultExp = resultExp - HeroStat.MaxExp;
			BaseHeroStat.Level += 1;
			HeroStat.Level += 1;
			if (HeroStat.Level >= 9)
			{
				BaseHeroStat.MaxExp = HeroStat.RequiresExp[9];
			}
			else
			{
				BaseHeroStat.MaxExp = HeroStat.RequiresExp[HeroStat.Level];
			}
			HeroStat.MaxExp = BaseHeroStat.MaxExp;
		}
		
		HeroStat.Exp = resultExp;
		return true;
	}
	HeroStat.Exp = resultExp;
	return false;
}
