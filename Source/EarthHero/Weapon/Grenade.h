// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class EARTHHERO_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrenade();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	float GrenadeDamage;

	float GrenadeRange;

	UFUNCTION()
	void GrenadeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(Server, Reliable)
	void Server_GrenadeHit(FVector HitLocation);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_GrenadeHit(FVector HitLocation);
	
public:
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() { return ProjectileMovementComponent; }

	FORCEINLINE float GetGrenadeDamage() { return GrenadeDamage; }
	FORCEINLINE float GetGrenadeRange() { return GrenadeRange; }

	FORCEINLINE void SetGrenadeDamage(float _GrenadeDamage) { GrenadeDamage = _GrenadeDamage; }
	FORCEINLINE void SetGrenadeRange(float _GrenadeRange) { GrenadeRange = _GrenadeRange; }
};
