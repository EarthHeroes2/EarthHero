// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthHero/Stat/Effect/Ef_Bind.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Enum/Enums.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEf_Bind::ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable,
                           bool InbIsPermanent, bool InbShouldRefreshDuration)
{
	EffectType = Ef_Bind;
	Super::ApplyEffect(InTargetActor, InEffectValue, InDuration, InbIsStackable, InbIsPermanent,
	                   InbShouldRefreshDuration);

	//적용 대상이 캐릭터이면..
	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	{
		if (Character->GetCharacterMovement())
		{
			// 기존 이동 속도를 저장
			OriginalMaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
			OriginalMaxFlySpeed = Character->GetCharacterMovement()->MaxFlySpeed;
			//OriginalMaxVelocity = Character->GetCharacterMovement()->Velocity;

			// 이동 속도를 0으로 설정 
			Character->SetMaxWalkSpeed(0);
			Character->SetMaxFlySpeed(0);
			//Character->SetVelocity(FVector(0, 0, 0));

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
			OriginalMaxFlySpeed = Monster->GetCharacterMovement()->MaxFlySpeed;
			OriginalMaxVelocity = Monster->GetCharacterMovement()->Velocity;

			// 이동 속도를 0으로 설정
			Monster->SetMaxWalkSpeed(0);
			Monster->SetMaxFlySpeed(0);
			Monster->SetIsCanMove(false);
			Monster->SetVelocity(FVector(0, 0, 0));

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
			Character->SetMaxWalkSpeed(OriginalMaxWalkSpeed);
			Character->SetMaxFlySpeed(OriginalMaxFlySpeed);
			//Character->SetVelocity(OriginalMaxVelocity);
		}
	}
	else if (AMonsterBase* Monster = Cast<AMonsterBase>(TargetActor))
	{
		if (Monster->GetCharacterMovement())
		{
			Monster->SetMaxWalkSpeed(OriginalMaxWalkSpeed);
			Monster->SetMaxFlySpeed(OriginalMaxFlySpeed);
			Monster->SetIsCanMove(true);
			Monster->SetVelocity(OriginalMaxVelocity);
		}
	}
	
	Super::ResetEffect();
}
