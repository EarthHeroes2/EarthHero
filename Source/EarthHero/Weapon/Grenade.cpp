// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "Components/SphereComponent.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/DamageType/NormalDamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(SphereComponent);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::GrenadeHit);
}

void AGrenade::GrenadeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(GetNetMode() != NM_Client)
	{
		const FVector HitLocation = Hit.Location;
		Server_GrenadeHit(HitLocation);
	}
}


void AGrenade::Server_GrenadeHit_Implementation(FVector HitLocation)
{
	//스텟 컴포넌트는 서버에서만 접근 권장
	GrenadeRange = ShooterStatComponent->SH_GrenadeRange;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<FHitResult> HitArray;
	
	UWorld* World = GetWorld();
	if(World)
	{
		const bool bHit = UKismetSystemLibrary::SphereTraceMulti(World, HitLocation, HitLocation, GrenadeRange,
			UEngineTypes::ConvertToTraceType(ECC_Pawn), false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, HitArray, true, FLinearColor::Gray, FLinearColor::Blue, 5.f);
	
		if(bHit)
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
					ShooterStatComponent->ShooterGrenadeDamage(HitMonster);
				}
			}
		}
	}
	
	NetMulticast_GrenadeHit(HitLocation);
}


void AGrenade::NetMulticast_GrenadeHit_Implementation(FVector HitLocation)
{
	//애니메이션 재생
	Destroy();
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

