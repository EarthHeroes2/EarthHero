// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorStatComponent.h"

#include "StatCalculationLibrary.h"
#include "DamageType/NormalDamageType.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Character/Warrior/EHWarrior.h"
#include "EarthHero/Character/Warrior/WarriorCombatComponent.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "Monster/MonsterStatComponent.h"

//워리어 생성자
UWarriorStatComponent::UWarriorStatComponent()
{
}

void UWarriorStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarriorStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWarriorStatComponent::OnRep_HeroStat()
{
	Super::OnRep_HeroStat();
	
	if (Warrior && Warrior->CombatComponent)
	{
		Warrior->CombatComponent->SetSkillCoolDown(HeroStat.SkillCoolTime);
		Warrior->CombatComponent->SetDashCoolDown(HeroStat.DashCoolTime);
	}
	if (Warrior)
	{
		//서버도 설정해주게 바꿔야함
		Warrior->SetMaxWalkSpeed(600 * HeroStat.MovementSpeed);
		//UE_LOG(LogClass, Warning, TEXT("walkSpeed : %f"), Warrior->GetCharacterMovement()->MaxWalkSpeed);
	}
}

//워리어 데미지 받는 함수(오버라이드)


//워리어 스텟 초기화 함수(오버라이드)
void UWarriorStatComponent::InitializeStatData_Implementation()
{
	Super::InitializeStatData_Implementation();
}

////////////////
/*워리어 용 함수*/
///////////////


//워리어 일반데미지
void UWarriorStatComponent::WarriorDamage_Implementation(AActor* DamagedActor)
{
	float resultDamage = UStatCalculationLibrary::CalWarriorNormalDamage(HeroStat, WR_NormalDamage);
	float actualDamage = 0;
	
	if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
	{
		static FHitResult DummyHitResult;
		actualDamage = HitMonster->MonsterStatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), DummyHitResult, nullptr, Warrior, IsDead);
		if (IsDead)
		{
			KillCount += 1;
			IsDead = false;
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdateGameStateKillCount();
			}
		}

		if (actualDamage > 0)
		{
			GivenDamage += actualDamage;
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdateGameStateDamage();
			}

			if (WR_Drain > 0)
			{
				if (HeroStat.Health + actualDamage * WR_Drain > HeroStat.MaxHealth)
				{
					HeroStat.Health = HeroStat.MaxHealth;
				}
				else
				{
					HeroStat.Health += actualDamage * WR_Drain; 
				}
			}
		}
		
		UE_LOG(LogTemp, Error, TEXT("Acture Damage = %f"), actualDamage);
	}
}

//워리어 휠윈드
void UWarriorStatComponent::WarriorWheelWindDamage_Implementation(AActor* DamagedActor)
{
	float resultDamage = UStatCalculationLibrary::CalWarriorWheelWindDamage(HeroStat, WR_WheelWindDamage);
	float actualDamage = 0;
	
	if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
	{
		static FHitResult DummyHitResult;
		actualDamage = HitMonster->MonsterStatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), DummyHitResult, nullptr, Warrior, IsDead);
		if (IsDead)
		{
			KillCount += 1;
			IsDead = false;
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdateGameStateKillCount();
			}
		}

		if (actualDamage > 0)
		{
			GivenDamage += actualDamage;
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdateGameStateDamage();
			}

			if (WR_Drain > 0)
			{
				if (HeroStat.Health + actualDamage * WR_Drain > HeroStat.MaxHealth)
				{
					HeroStat.Health = HeroStat.MaxHealth;
				}
				else
				{
					HeroStat.Health += actualDamage * WR_Drain; 
				}
			}
		}
		
		UE_LOG(LogTemp, Error, TEXT("Acture Damage = %f"), actualDamage);
	}
}

//워리어 대쉬
void UWarriorStatComponent::WarriorDashDamage_Implementation(AActor* DamagedActor)
{
	float resultDamage = UStatCalculationLibrary::CalWarriorDashDamage(HeroStat, WR_JumpDamageMulti, WR_NormalDamage);
	float actualDamage = 0;
	
	if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
	{
		static FHitResult DummyHitResult;
		actualDamage = HitMonster->MonsterStatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), DummyHitResult, nullptr, Warrior, IsDead);
		if (IsDead)
		{
			KillCount += 1;
			IsDead = false;
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdateGameStateKillCount();
			}
		}

		if (actualDamage > 0)
		{
			GivenDamage += actualDamage;
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdateGameStateDamage();
			}

			if (WR_Drain > 0)
			{
				if (HeroStat.Health + actualDamage * WR_Drain > HeroStat.MaxHealth)
				{
					HeroStat.Health = HeroStat.MaxHealth;
				}
				else
				{
					HeroStat.Health += actualDamage * WR_Drain; 
				}
			}
		}
		
		UE_LOG(LogTemp, Error, TEXT("Acture Damage = %f"), actualDamage);
	}
}
