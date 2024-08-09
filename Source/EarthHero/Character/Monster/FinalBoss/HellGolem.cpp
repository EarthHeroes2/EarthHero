// Fill out your copyright notice in the Description page of Project Settings.


#include "HellGolem.h"

#include "EarthHero/Stat/Effect/Ef_VisibleBlocking.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"
#include "Kismet/GameplayStatics.h"

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
	StartLocationSocket = FName("RightMeleeStart");
	EndLocationSocket = FName("RigthMeleeEnd");
	
	Super::Attack();
	
	MC_PlayAttackMontage();
}

void AHellGolem::MC_PlayAttackMontage_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    
	if (AnimInstance && AttackAnimMontage)
	{
		// 애니메이션 몽타주를 재생합니다.
		AnimInstance->Montage_Play(AttackAnimMontage);
	}
}

//[스킬 1] 일정 범위 내 모든 캐릭터들이 3초동안 자신의 주변 좁은 범위만 볼 수 있게 함.
//(쿨타임 : 19초)
void AHellGolem::Skill1()
{
	Super::Skill1();

	// 현재 액터의 위치
	FVector MyLocation = GetActorLocation();

	// 모든 타겟 클래스의 액터들을 찾습니다.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEHCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor != nullptr)
		{
			float Distance = FVector::Dist(MyLocation, Actor->GetActorLocation());

			if (Distance <= 2000.f)
			{
				SpawnEffects(Actor);
			}
		}
	}
}

void AHellGolem::Skill2()
{
	Super::Skill2();
}

void AHellGolem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHellGolem::SpawnEffects_Implementation(AActor *Actor)
{
	UE_LOG(LogClass, Error, TEXT("SpawmEffects1"));
	if (AEf_VisibleBlocking* Ef_VisibleBlocking = GetWorld()->SpawnActor<AEf_VisibleBlocking>())
	{
		UE_LOG(LogClass, Error, TEXT("SpawmEffects2"));
		Ef_VisibleBlocking->ApplyEffect(Actor, 0, 3, false, false, true); // 중첩 불가능, 영구 아님, 지속 시간 갱신 가능
	}
}

