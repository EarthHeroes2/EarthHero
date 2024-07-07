// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "GameFramework/ProjectileMovementComponent.h"

AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

