

#include "WarriorCombatComponent.h"

#include "EHWarrior.h"
#include "Camera/CameraComponent.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UWarriorCombatComponent::UWarriorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWarriorCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Warrior = Cast<AEHWarrior>(GetOwner());
}

void UWarriorCombatComponent::Attack()
{
	if(!Warrior)
	{
		Warrior = Cast<AEHWarrior>(GetOwner());
	}

	float PlayRate = FMath::Clamp(0.3f * (1.f / AttackCooldown), 0.6f, 1.2f);
	
	if(Warrior && bCanAttack && !bIsWhirlwind)
	{
		if(Warrior->IsLocallyControlled())
		{
			if(FPS_AttackAnimMontage[AttackCombo])
			{
				UAnimInstance* WarriorAnimInstance = Warrior->GetFirstPersonMesh()->GetAnimInstance();
				WarriorAnimInstance->Montage_Play(FPS_AttackAnimMontage[AttackCombo]);
				WarriorAnimInstance->Montage_SetPlayRate(FPS_AttackAnimMontage[AttackCombo], PlayRate);
			}
		}

		AttackCombo  = (AttackCombo + 1) % FPS_AttackAnimMontage.Num();

		bCanAttack = false;
		Warrior->GetWorldTimerManager().SetTimer(AttackCooldownTimerHandle,this, &ThisClass::ResetAttack, AttackCooldown);
	
		Server_Attack();
	}
}

void UWarriorCombatComponent::Server_Attack_Implementation()
{
	
	NetMulticast_Attack();
}

void UWarriorCombatComponent::NetMulticast_Attack_Implementation()
{
	//TPS
	if(!Warrior->IsLocallyControlled())
	{
		if(TPS_AttackAnimMontage)
		{
			Warrior->GetMesh()->GetAnimInstance()->Montage_Play(TPS_AttackAnimMontage);
		}
	}
}

void UWarriorCombatComponent::ResetAttack()
{
	bCanAttack = true;
}

void UWarriorCombatComponent::SwordHit()
{
	if(Warrior && Warrior->GetFPSCamera())
	{
		Server_SwordHit(Warrior->GetFPSCamera()->GetComponentLocation(),
			Warrior->GetFPSCamera()->GetComponentRotation());
	}
}

void UWarriorCombatComponent::Server_SwordHit_Implementation(FVector CamLocation, FRotator CamRotation)
{
	FVector EndLocation = CamLocation + (CamRotation.Vector() * 180.f);
	FVector HalfSize(0.f,150.f,0.f);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Warrior);
	TArray<FHitResult> HitArray;
	
	UWorld* World = GetWorld();
	if(World)
	{
		const bool bHit = UKismetSystemLibrary::BoxTraceMulti(World, CamLocation, EndLocation, HalfSize, CamRotation,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), false, ActorsToIgnore, EDrawDebugTrace::None,
			HitArray, true, FLinearColor::Green, FLinearColor::Red, 5.f);

		
		if(HitArray.Num() != 0)
		{
			TSet<AActor*> UniqueActors;

			for(const FHitResult HitResult : HitArray)
			{
				if(UniqueActors.Contains(HitResult.GetActor()))
				{
					continue;
				}
				UniqueActors.Add(HitResult.GetActor());
				
				if(AMonsterBase* HitMonster = Cast<AMonsterBase>(HitResult.GetActor()))
				{
					// 일반 공격 데미지 주는 함수 호출 (Server)

					// 공격 파티클 생성 (NetMulticast)
					NetMulticast_SwordHit(HitResult);
				}
			}
		}
	}
}

void UWarriorCombatComponent::NetMulticast_SwordHit_Implementation(FHitResult HitResult)
{
	UWorld* World = GetWorld();
	if(World)
	{
		if(SwordHitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, SwordHitParticle, HitResult.ImpactPoint);
		}
	}
	
}


void UWarriorCombatComponent::ToggleWhirlwind()
{
	if(bCanWhirlwind)
	{
		bIsWhirlwind = true;
		bCanWhirlwind = false;
		CurrentWhirlwindCount = 0;
		Warrior->GetWorldTimerManager().SetTimer(WhirlwindCooldownTimerHandle, this, &ThisClass::ResetWhirlWind, WhirlwindCooldown);
		Warrior->GetWorldTimerManager().SetTimer(WhirlwindTimerHandle, this, &ThisClass::Whirlwind, 0.25f, true);
	}
	else
	{
		if(bIsWhirlwind)
		{
			bIsWhirlwind = false;
		}
	}
}

void UWarriorCombatComponent::Whirlwind()
{
	if(!bIsWhirlwind)
	{
		Warrior->GetWorldTimerManager().ClearTimer(WhirlwindTimerHandle);
		return;
	}
	
	if(CurrentWhirlwindCount <= TotalWhirlwindCount)
	{
		CurrentWhirlwindCount++;

		if(Warrior && Warrior->GetFPSCamera())
		{
			FVector CamLocation = Warrior->GetFPSCamera()->GetComponentLocation();
			Server_Whirlwind(CamLocation);
		}
	}
	else
	{
		Warrior->GetWorldTimerManager().ClearTimer(WhirlwindTimerHandle);
		bIsWhirlwind = false;
	}
}

void UWarriorCombatComponent::Server_Whirlwind_Implementation(FVector CamLocation)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Warrior);
	TArray<FHitResult> HitArray;

	UWorld* World = GetWorld();
	if(World)
	{
		// Sphere Trace 사용하긴 하지만 나중에 바꾸고 싶음. (Cylinder Trace느낌으로?)
		const bool bHit = UKismetSystemLibrary::SphereTraceMulti(World, CamLocation, CamLocation, 200.f,
			 UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), false, ActorsToIgnore,
			 EDrawDebugTrace::None, HitArray, true, FLinearColor::Green, FLinearColor::Red, 4.f);

		if(HitArray.Num() != 0)
		{
			TSet<AActor*> UniqueActors;

			for(const FHitResult HitResult : HitArray)
			{
				if(UniqueActors.Contains(HitResult.GetActor()))
				{
					continue;
				}
				UniqueActors.Add(HitResult.GetActor());

				if(AMonsterBase* HitMonster = Cast<AMonsterBase>(HitResult.GetActor()))
				{
					FVector Direction = HitMonster->GetActorLocation() - Warrior->GetActorLocation();
					Direction.Z = 0.f;
					HitMonster->LaunchCharacter(Direction * 1.f, false, false);
					// 공격 파티클 생성 (NetMulticast)
					NetMulticast_Whirlwind(HitResult);
				}
			}
		}
	}
}

void UWarriorCombatComponent::NetMulticast_Whirlwind_Implementation(FHitResult HitResult)
{
	UWorld* World = GetWorld();
	if(World)
	{
		if(WhirlwindHitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, SwordHitParticle, HitResult.ImpactPoint);
		}
	}
}

void UWarriorCombatComponent::ResetWhirlWind()
{
	bCanWhirlwind = true;
}