// Fill out your copyright notice in the Description page of Project Settings.


#include "HellGolem.h"

#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AHellGolem::AHellGolem()
{
	MonsterType = Boss;
	BossNumber = MainBoss;

	// 평타 공격 Range
	AttackRange = 500;
}

//평타 구현
void AHellGolem::Attack()
{
	Super::Attack();
	MC_PlayAttackMontage();
}

void AHellGolem::MC_PlayAttackMontage_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("PlayingAttackMontage"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    
	if (AnimInstance && MeleeAttackMontage)
	{
		// 애니메이션 몽타주를 재생합니다.
		AnimInstance->Montage_Play(MeleeAttackMontage);
	}
}

void AHellGolem::Skill1()
{
	Super::Skill1();
}

void AHellGolem::Skill2()
{
	Super::Skill2();
}

void AHellGolem::Tick(float DeltaSeconds)
{
	//Super::Tick(DeltaSeconds);
	APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayingGameMode)
	{
		if (PlayingGameMode->IsDebugMode)
		{
			if (GetNetMode() == NM_ListenServer)
			{
				DoMeleeTrace();
			}
		}
		else
		{
			if (IsRunningDedicatedServer())
			{
				DoMeleeTrace();
			}
		}
	}
	
}

void AHellGolem::DoMeleeTrace()
{
	if (CanMeleeAttack)
	{
		FVector Start = GetMesh()->GetSocketLocation(TEXT("RightMeleeStart"));
		FVector End = GetMesh()->GetSocketLocation(TEXT("RigthMeleeEnd"));
		
		TArray<FHitResult> OutHits;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel2));

		// 트레이스할 액터를 무시할 배열
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		// 스피어 트레이스 반지름 설정
		float SphereRadius = 150.0f;

		// 트레이스 실행
		bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
			SphereRadius,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHits,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			6.0f
		);

		// 트레이스 결과 처리
		if (bHit)
		{
			for (auto& Hit : OutHits)
			{
				CheckAttackedEnemy(Hit);
			}
		}
		
	}
	else
	{
		AttackedEnemy.Empty();
	}
}

void AHellGolem::CheckAttackedEnemy(FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor && !AttackedEnemy.Contains(HitActor))
	{
		AttackedEnemy.Add(HitActor);
		
		if (HitActor)
		{
			// 데미지 적용
			MonsterStatComponent->GiveNormalDamage(HitActor, MeleeDamage);
		}
	}
}
