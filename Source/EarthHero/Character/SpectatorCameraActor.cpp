// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorCameraActor.h"

#include "Components/ArrowComponent.h"

// Sets default values
ASpectatorCameraActor::ASpectatorCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpectatorCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectatorCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

