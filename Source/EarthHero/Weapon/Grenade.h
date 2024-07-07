// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

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
	UStaticMeshComponent* MeshComponent;

	float GrenadeDamage;
	
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() { return ProjectileMovementComponent; }
};
