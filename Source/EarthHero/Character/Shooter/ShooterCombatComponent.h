// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EarthHero/EHComponents/CombatComponent.h"
#include "ShooterCombatComponent.generated.h"


class AEHShooter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UShooterCombatComponent : public UCombatComponent
{
	GENERATED_BODY()

public:	
	UShooterCombatComponent();

	void Fire();

	void GrenadeFire();
	
protected:
	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector TraceStartVector, FVector TraceEndVector, FVector MuzzleVector);
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Fire(FVector HitLocation, FVector MuzzleVector);

	virtual void BeginPlay() override;
	
	void ResetFire();

	UFUNCTION(Server, Reliable)
	void Server_GrenadeFire();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_GrenadeFire();

private:
	UPROPERTY()
	AEHShooter* Shooter;

	FTimerHandle ShootTimerHandle;

	UPROPERTY()
	bool bCanFire = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Fire")
	float FireRate = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* BulletHitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Weapon")
	TSubclassOf<class AGrenade> GrenadeClass;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "AnimMontage")
	UAnimMontage* FPS_FireMontage;
	
	
public:
	FORCEINLINE void SetShooter(AEHShooter* NewShooter) { Shooter = NewShooter; }

	UFUNCTION(Client, Reliable)
	void SetFireRate(float NewFireRate);
};
