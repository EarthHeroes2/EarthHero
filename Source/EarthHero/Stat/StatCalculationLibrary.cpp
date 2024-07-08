

#include "StatCalculationLibrary.h"

#include "Structure/StatStructure.h"

void UStatCalculationLibrary::CalNormalDamage(FStatStructure& HeroStat, float inDamage)
{
	//데미지 0이하 일 시 무시
	if (inDamage <= 0)
		return ;

	// 남는 방어력이 있으면,
	if (HeroStat.DefensePower > 0)
	{
		float leftDamage = HeroStat.DefensePower - inDamage;

		//방어력을 넘는 데미지가 들어올 시
		if (leftDamage > 0)
		{
			HeroStat.DefensePower = 0;
			//실질 데미지 = 체력 - 데미지 
			float RealDamage = HeroStat.Health - leftDamage;
			if (RealDamage > 0)
			{
				HeroStat.Health -= leftDamage;
			}
			else
			{
				HeroStat.Health = 0;
			}
		}
		else
		{
			HeroStat.DefensePower -= inDamage;
		}
	}
	else
	{
		//실질 데미지 = 체력 - 데미지 
		float RealDamage = HeroStat.Health - inDamage;
		if (RealDamage > 0)
		{
			HeroStat.Health -= inDamage;
		}
		else
		{
			HeroStat.Health = 0;
		}
	}
}

float UStatCalculationLibrary::CalShooterNormalDamage(FStatStructure &HeroStat, const FHitResult & HitInfo, int32 SH_HeadShot)
{
	float resultDamage = 0.f;
	if (HitInfo.BoneName == "head")
	{
		resultDamage = HeroStat.NormalDamage * SH_HeadShot;
	}
	else
	{
		resultDamage = HeroStat.NormalDamage;
	}
	return resultDamage;
}

bool UStatCalculationLibrary::AddExp(FStatStructure& HeroStat, int32 ExpMount)
{
	float resultExp = HeroStat.Exp + ExpMount;
	if (resultExp >= HeroStat.MaxExp)
	{
		while (resultExp >= HeroStat.MaxExp)
		{
			resultExp = resultExp - HeroStat.MaxExp;
			HeroStat.Level += 1;
			HeroStat.MaxExp = HeroStat.RequiresExp[HeroStat.Level];
		}
		
		HeroStat.Exp = resultExp;
		return true;
	}
	HeroStat.Exp = resultExp;
	return false;
}
