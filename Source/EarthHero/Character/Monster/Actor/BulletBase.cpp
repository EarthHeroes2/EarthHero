// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"

#include "Components/CapsuleComponent.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->InitCapsuleSize(50.f, 50.0f);
	RootComponent = CapsuleComp;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	StaticMeshComp->SetupAttachment(CapsuleComp);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create Projectile Movement Component
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->UpdatedComponent = CapsuleComp;
	ProjectileMovementComp->InitialSpeed = 1000.f;
	ProjectileMovementComp->MaxSpeed = 1000.f;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	if(GetNetMode() != NM_Client) CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlapBegin);
}

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = GetOwner();
	if(Actor)
		MonsterBase = Cast<AMonsterBase>(Actor);
	
}

void ABulletBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
		{
			if(MonsterBase)
			{
				UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), BulletDamage);
				MonsterBase->MonsterStatComponent->GiveNormalDamage(Character, BulletDamage);
			}
		}
		Destroy();
	}
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

