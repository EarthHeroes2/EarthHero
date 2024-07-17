// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthHero/Stat/Effect/Ef_Bind.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEf_Bind::ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable,
                           bool InbIsPermanent, bool InbShouldRefreshDuration)
{
	Super::ApplyEffect(InTargetActor, InEffectValue, InDuration, InbIsStackable, InbIsPermanent,
	                   InbShouldRefreshDuration);

	//적용 대상이 캐릭터이면..
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		if (Character->GetCharacterMovement())
		{
			// 기존 이동 속도를 저장
			OriginalMaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;

			// 이동 속도를 0으로 설정
			Character->GetCharacterMovement()->MaxWalkSpeed = 0.0f;

			// 이 액터를 대상 액터에 부착
			AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		if (Monster->GetCharacterMovement())
		{
			// 기존 이동 속도를 저장
			OriginalMaxWalkSpeed = Monster->GetCharacterMovement()->MaxWalkSpeed;

			// 이동 속도를 0으로 설정
			Monster->GetCharacterMovement()->MaxWalkSpeed = 0.0f;

			// 이 액터를 대상 액터에 부착
			AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AEf_Bind::UpgradeEffect(float InEffectValue)
{
	Super::UpgradeEffect(InEffectValue);

	if (bRefresh && !bIsStackable)
	{
		bRefresh = false;
		return;
	}
}

void AEf_Bind::ResetEffect()
{
	//적용 대상이 캐릭터이면..
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		if (Character->GetCharacterMovement())
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
		}
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		if (Monster->GetCharacterMovement())
		{
			Monster->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
		}
	}
	
	Super::ResetEffect();
}
