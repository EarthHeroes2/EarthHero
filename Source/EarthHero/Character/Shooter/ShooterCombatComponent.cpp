// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCombatComponent.h"

#include "EHShooter.h"
#include "Camera/CameraComponent.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/DamageType/NormalDamageType.h"
#include "EarthHero/Weapon/Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UShooterCombatComponent::UShooterCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShooterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	Shooter = Cast<AEHShooter>(GetOwner());
}

void UShooterCombatComponent::Fire()
{
	if (!Shooter) return;
	if(bCanFire)
	{
		bCanFire = false;

		if (!GetOwner()->GetInstigatorController())
		{
			UE_LOG(LogClass, Warning, TEXT("No Shooters Controller"));
			return;
		}
		FRotator CameraRotation = Shooter->GetInstigatorController()->GetControlRotation();
		FVector StartLocation = Shooter->GetFPSCamera()->GetComponentLocation();
		FVector EndLocation = StartLocation + (CameraRotation.Vector() * 15000.f);
		FVector MuzzleLocation = Shooter->GetEquippedWeapon()->GetSocketLocation(FName("FireSocket"));
		
		Server_Fire(StartLocation, EndLocation, MuzzleLocation);
		
		Shooter->GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ThisClass::ResetFire, FireRate, false);
	}
}

void UShooterCombatComponent::GrenadeFire()
{
	if(Shooter)
	{
		Server_GrenadeFire();
	}
}


void UShooterCombatComponent::Server_GrenadeFire_Implementation()
{
	NetMulticast_GrenadeFire();
}

void UShooterCombatComponent::NetMulticast_GrenadeFire_Implementation()
{
	UWorld* World = GetWorld();
	UObject* GrenadeBPObject = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/Weapons/BP_Grenade.BP_Grenade"));
	UBlueprint* GrenadeBP = Cast<UBlueprint>(GrenadeBPObject);
	TSubclassOf<UObject> GrenadeBPClass = (UClass*)GrenadeBP->GeneratedClass;
	
	if(World && GrenadeBPClass)
	{
		if(Shooter && Shooter->GetEquippedWeapon() && Shooter->GetController())
		{
			FVector SpawnLocation = Shooter->GetEquippedWeapon()->GetSocketLocation(FName("FireSocket"));
			FRotator SpawnRotation = Shooter->GetController()->GetControlRotation();
			SpawnRotation += FRotator(3.f,0.f,0.f);
			AActor* SpawnGrenade = World->SpawnActor<AActor>(GrenadeBPClass, SpawnLocation, SpawnRotation);
			AGrenade* Grenade = Cast<AGrenade>(SpawnGrenade);
			if(Grenade)
			{
				Grenade->GetProjectileMovementComponent()->InitialSpeed = 4000.f;
				
				//승언 : Grenade에 ShooterStatComponent 참조 추가
				Grenade->ShooterStatComponent = Shooter->ShooterStatComponent;
			}
		}
	}

}


void UShooterCombatComponent::Server_Fire_Implementation(FVector TraceStartVector, FVector TraceEndVector,
                                                         FVector MuzzleVector)
{
	FHitResult HitResult;
	bool bHit = false;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(Shooter);
	
	UWorld* World = GetWorld();
	if(World)
	{
		bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartVector, TraceEndVector,
			ECC_GameTraceChannel1, collisionParams);
	}
	
	if(bHit)
	{
		//07.09 AEHCharacter -> Monster로 변경
		AMonsterBase *HitActor = Cast<AMonsterBase>(HitResult.GetActor());
		if (!Shooter)
		{
			UE_LOG(LogClass, Warning, TEXT("ShooterCombatComponent::Fire() : No Shooter"));
			return;
		}
		if (Shooter && !Shooter->ShooterStatComponent)
		{
			UE_LOG(LogClass, Warning, TEXT("ShooterCombatComponent::Fire() : No ShooterStatComponent"));
			return;
		}
		if (HitActor && Shooter && Shooter->ShooterStatComponent)
		{
			Shooter->ShooterStatComponent->ShooterDamage(HitActor, HitResult, UNormalDamageType::StaticClass(), Shooter);
		}
		// TODO
		// a. If Target is an Enemy, Call the Damage Function

		NetMulticast_Fire(HitResult.Location, MuzzleVector);
	}
	else
	{
		NetMulticast_Fire(FVector::Zero(), MuzzleVector);
	}
}

void UShooterCombatComponent::NetMulticast_Fire_Implementation(FVector HitLocation, FVector MuzzleVector)
{
	if(FireParticle && BulletHitParticle)
	{
		
		FVector MuzzleLocation = Shooter->GetEquippedWeapon()->GetSocketLocation(FName("FireSocket"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, MuzzleLocation);
		if(HitLocation != FVector::Zero())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitParticle,HitLocation);
		}
	}

	if(Shooter->IsLocallyControlled())
	{
		if(FPS_FireMontage && Shooter->GetFirstPersonMesh() && Shooter->GetFirstPersonMesh()->GetAnimInstance())
		{
			Shooter->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(FPS_FireMontage);
		}
	}
}


void UShooterCombatComponent::ResetFire()
{
	bCanFire = true;
}
