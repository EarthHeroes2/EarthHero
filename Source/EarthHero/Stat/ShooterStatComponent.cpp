// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterStatComponent.h"
#include "StatCalculationLibrary.h"
#include "DamageType/NormalDamageType.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Character/Shooter/EHShooter.h"
#include "EarthHero/Character/Shooter/ShooterCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Monster/MonsterStatComponent.h"


void UShooterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UShooterStatComponent::CalFireRate()
{
	return 0.1f / HeroStat.AttackSpeed;
}

void UShooterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UShooterStatComponent::OnRep_HeroStat()
{
	Super::OnRep_HeroStat();
	if (GetNetMode() != NM_Client && Shooter && Shooter->CombatComponent)
	{
		Shooter->CombatComponent->SetFireRate(CalFireRate());
	}
}


////////////////
//*슈터 용 함수*//
///////////////

//슈터 일반 데미지
void UShooterStatComponent::ShooterDamage_Implementation(AActor* DamagedActor, const FHitResult& HitInfo,
	TSubclassOf<UDamageType> DamageTypeClass, AEHCharacter* DamageCusor)
{
	float resultDamage = UStatCalculationLibrary::CalShooterNormalDamage(HeroStat, HitInfo, SH_HeadShot, SH_NormalDamage);
	float actualDamage = 0;

	if (resultDamage > 0)
	{
		//몬스터용 데미지 입는 함수 호출
		if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
		{
			actualDamage = HitMonster->MonsterStatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), HitInfo, nullptr, Shooter);
			UE_LOG(LogTemp, Error, TEXT("Acture Damage = %f"), actualDamage);
		}
		else if (AEHCharacter *HitHero = Cast<AEHCharacter>(DamagedActor)) // 임시 슈터끼리 공격
		{
			actualDamage = HitHero->StatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), HitInfo, nullptr, Shooter);
			UE_LOG(LogTemp, Error, TEXT("Acture Damage = %f"), actualDamage);
		}
	}
}

void UShooterStatComponent::ShooterGrenadeDamage_Implementation(AActor* DamagedActor)
{
	float resultDamage = UStatCalculationLibrary::CalShooterGrenadeDamage(HeroStat, SH_GrenadeDamage);
	float actualDamage = 0;
	
	if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
	{
		static FHitResult DummyHitResult;
		actualDamage = HitMonster->MonsterStatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), DummyHitResult, nullptr, Shooter);
		UE_LOG(LogTemp, Error, TEXT("Acture Damage = %f"), actualDamage);
	}
}

