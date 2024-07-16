// Fill out your copyright notice in the Description page of Project Settings.


#include "Ef_PointImpulse.h"

#include <corecrt_io.h>

#include "Chaos/Utilities.h"
#include "Components/CapsuleComponent.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Enum/Enums.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AEf_PointImpulse::AEf_PointImpulse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// RootComponent를 설정합니다.
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForceComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEf_PointImpulse::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
	{
		RadialForceComponent->ImpulseStrength = Strength;
		RadialForceComponent->Radius = Radius;
		RadialForceComponent->bImpulseVelChange = true;
		GetWorldTimerManager().SetTimer(ImpulseHandle, this, &ThisClass::PointImpulse, Delay, true);
	}
}

void AEf_PointImpulse::PointImpulse()
{
	if (Duration < Time)
	{
		GetWorldTimerManager().ClearTimer(ImpulseHandle);
		GetWorldTimerManager().SetTimer(SetFalseSimulatePhysicsHandle, this, &ThisClass::SetFalseSimulatePhysics, 1.5, false);
	}
	
	for (auto HitActor : HitActors)
	{
		if (AMonsterBase *Monster = Cast<AMonsterBase>(HitActor))
		{
			if (Monster->MonsterType != Boss)
			{
				MonsterArray.Add(Monster);

				//AIController가 아니라서 나중에 적용되는지 확인해봐야 함
				Monster->GetController()->StopMovement();
				Monster->GetCapsuleComponent()->SetSimulatePhysics(true);
				
				FRotator MonsterRotation = Monster->GetControlRotation();
				
				// 회전 행렬을 통해 벡터를 회전시킵니다.
				FRotator RotatedVector = FRotator(MonsterRotation.Vector().X, MonsterRotation.Vector().Y, 0);
				Monster->SetActorRotation(RotatedVector);
			}
		}
	}
	Time += Delay;
	GetWorldTimerManager().SetTimer(DelayHandle, this, &ThisClass::Impulse, 0.1, false);
}

void AEf_PointImpulse::Impulse() const
{
	RadialForceComponent->FireImpulse();
}

void AEf_PointImpulse::SetFalseSimulatePhysics()
{
	for (auto Monster : MonsterArray)
	{
		if (IsValid(Monster))
		{
			//AIController가 아니라서 나중에 적용되는지 확인해봐야 함
			Monster->GetController()->StopMovement();
			Monster->GetCapsuleComponent()->SetSimulatePhysics(false);
			
			FRotator MonsterRotation = Monster->GetControlRotation();

			// 회전 행렬을 통해 벡터를 회전시킵니다.
			FRotator RotatedVector = FRotator(MonsterRotation.Vector().X, MonsterRotation.Vector().Y, 0);
			Monster->SetActorRotation(RotatedVector);
		}
	}

	Destroy();
}
