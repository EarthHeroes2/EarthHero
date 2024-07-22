// Fill out your copyright notice in the Description page of Project Settings.


#include "Ef_ReduceDamageTaken.h"

#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"

void AEf_ReduceDamageTaken::ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration,
                                        bool InbIsStackable, bool InbIsPermanent, bool InbShouldRefreshDuration)
{
	Super::ApplyEffect(InTargetActor, InEffectValue, InDuration, InbIsStackable, InbIsPermanent,
	                   InbShouldRefreshDuration);

	if (bRefresh && !bIsStackable)
	{
		bRefresh = false;
		return;
	}
	//적용 대상이 캐릭터이면..
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		Character->StatComponent->GetHeroStat().MoreDamageTaken -= InEffectValue;

		// 이 액터를 대상 액터에 부착
		AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		Monster->MonsterStatComponent->GetMonsterStat().MoreDamageTaken -= InEffectValue;
		AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AEf_ReduceDamageTaken::UpgradeEffect(float InEffectValue)
{
	Super::UpgradeEffect(InEffectValue);
	//적용 대상이 캐릭터이면..
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		Character->StatComponent->GetHeroStat().MoreDamageTaken -= InEffectValue;
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		Monster->MonsterStatComponent->GetMonsterStat().MoreDamageTaken -= InEffectValue;
	}
}

void AEf_ReduceDamageTaken::ResetEffect()
{
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		Character->StatComponent->GetHeroStat().MoreDamageTaken += AppliedEffectValue;
		//UE_LOG(LogClass, Warning, TEXT("MorDamageTaken : %f"), Character->StatComponent->GetHeroStat().MoreDamageTaken);
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		//몬스터 이동 속도 리셋
		Monster->MonsterStatComponent->GetMonsterStat().MoreDamageTaken += AppliedEffectValue;
	}
	
	Super::ResetEffect();
}
