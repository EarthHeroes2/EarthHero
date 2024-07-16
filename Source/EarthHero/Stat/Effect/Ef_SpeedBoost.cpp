// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthHero/Stat/Effect/Ef_SpeedBoost.h"

#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"

void AEf_SpeedBoost::ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable, bool InbIsPermanent, bool InbShouldRefreshDuration)
{
	Super::ApplyEffect(InTargetActor, InEffectValue, InDuration, InbIsStackable, InbIsPermanent, InbShouldRefreshDuration);

	//적용 대상이 캐릭터이면..
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		Character->StatComponent->GetHeroStat().MovementSpeed += InEffectValue;

		// 이 액터를 대상 액터에 부착
		AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		//몬스터 이동속도 증가 구현
		Monster->MonsterStatComponent->GetMonsterStat().MovementSpeed += InEffectValue;
		
		AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AEf_SpeedBoost::UpgradeEffect(float InEffectValue)
{
	Super::UpgradeEffect(InEffectValue);

	if (bRefresh && !bIsStackable)
	{
		bRefresh = false;
		return;
	}
	
	//적용 수치 갱신
	AppliedEffectValue += InEffectValue;
	
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		Character->StatComponent->GetHeroStat().MovementSpeed += InEffectValue;
		
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		//몬스터 이동속도 증가 구현
		Monster->MonsterStatComponent->GetMonsterStat().MovementSpeed += InEffectValue;
	}
}

void AEf_SpeedBoost::ResetEffect()
{
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		Character->StatComponent->GetHeroStat().MovementSpeed -= AppliedEffectValue;
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		Monster->MonsterStatComponent->GetMonsterStat().MovementSpeed -= AppliedEffectValue;
	}
	
	Super::ResetEffect();
}
