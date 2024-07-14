// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterStatComponent.h"
#include "StatCalculationLibrary.h"
#include "DamageType/NormalDamageType.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Character/Shooter/EHShooter.h"
#include "Kismet/KismetSystemLibrary.h"


void UShooterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShooterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	Shooter = Cast<AEHShooter>(GetOwner());
}


//슈터 스텟 초기화 함수(오버라이드)
void UShooterStatComponent::InitializeStatData_Implementation()
{
	Super::InitializeStatData_Implementation();
}

////////////////
//*슈터 용 함수*//
///////////////

//슈터 일반 데미지
void UShooterStatComponent::ShooterDamage_Implementation(AActor* DamagedActor, const FHitResult& HitInfo,
	TSubclassOf<UDamageType> DamageTypeClass, AEHCharacter* DamageCusor)
{
	float resultDamage = UStatCalculationLibrary::CalShooterNormalDamage(HeroStat, HitInfo, SH_HeadShot);
	float actualDamage = 0;

	if (resultDamage > 0)
	{
		//몬스터용 데미지 입는 함수 호출
		if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
		{
			UE_LOG(LogTemp, Error, TEXT("Monster Damaged"));
		}
		else if (AEHCharacter *HitHero = Cast<AEHCharacter>(DamagedActor)) // 임시 슈터끼리 공격
		{
			HitHero->StatComponent->DamageTaken(resultDamage, UNormalDamageType::StaticClass(), HitInfo, nullptr, Shooter);
		}
	}
}

void UShooterStatComponent::ShooterGrenadeDamage_Implementation(AActor* DamagedActor)
{
	UStatCalculationLibrary::CalShooterGrenadeDamage(HeroStat, SH_GrenadeDamage);

	if(AMonsterBase* HitMonster = Cast<AMonsterBase>(DamagedActor))
	{
		UE_LOG(LogTemp, Error, TEXT("Monster Damaged"));
	}
}

