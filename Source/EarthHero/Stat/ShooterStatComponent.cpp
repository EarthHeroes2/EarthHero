// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterStatComponent.h"
#include "StatCalculationLibrary.h"
#include "DamageType/NormalDamageType.h"
#include "EarthHero/Character/EHCharacter.h"
#include "Kismet/GameplayStatics.h"

//슈터 생성자
UShooterStatComponent::UShooterStatComponent()
{
}

void UShooterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShooterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

//슈터 데미지 받는 함수(오버라이드)


//슈터 스텟 초기화 함수(오버라이드)
void UShooterStatComponent::InitializeStatData_Implementation(FName HeroName)
{
	Super::InitializeStatData_Implementation(HeroName);
}

  //////////////
//*슈터 용 함수*//
////////////////

//슈터 일반 데미지
void UShooterStatComponent::ShooterDamage_Implementation(AActor* DamagedActor, const FHitResult& HitInfo,
	TSubclassOf<UDamageType> DamageTypeClass, AEHCharacter* DamageCusor)
{
	float resultDamage = UStatCalculationLibrary::CalShooterNormalDamage(HeroStat, HitInfo, SH_HeadShot);
	float actualDamage = 0;

	if (resultDamage > 0)
	{
		//몬스터용 데미지 입는 함수 호출
		//임시로 히어로끼리 데미지 입게 만들기
		Cast<AEHCharacter>(DamagedActor)->StatComponent->DamageTaken(resultDamage, DamageTypeClass, HitInfo, GetOwner()->GetInstigatorController(), Cast<AEHCharacter>(GetOwner()));
	}
}

//슈터 수류탄 공격
void UShooterStatComponent::ShooterGenerateDamage_Implementation(FVector Origin,  TSubclassOf<UDamageType> DamageTypeClass)
{
}
