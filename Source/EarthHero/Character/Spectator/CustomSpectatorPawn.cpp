// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSpectatorPawn.h"

#include "Camera/CameraComponent.h"
#include "EarthHero/Player/EHPlayerController.h"

ACustomSpectatorPawn::ACustomSpectatorPawn()
{
    SetReplicates(true);
}

void ACustomSpectatorPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACustomSpectatorPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
    Super::SetupPlayerInputComponent(InInputComponent);
    
    InInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ACustomSpectatorPawn::On1KeyPressed);
    InInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ACustomSpectatorPawn::On2KeyPressed);
    InInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ACustomSpectatorPawn::On3KeyPressed);
    InInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ACustomSpectatorPawn::On4KeyPressed);
    InInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &ACustomSpectatorPawn::On0KeyPressed);
}

void ACustomSpectatorPawn::BeginPlay()
{
    Super::BeginPlay();

    AController* MyController = GetController();
    if(MyController) EHPlayerController = Cast<AEHPlayerController>(MyController);
}

void ACustomSpectatorPawn::On1KeyPressed()
{
    if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(1);
}

void ACustomSpectatorPawn::On2KeyPressed()
{
    if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(2);
}

void ACustomSpectatorPawn::On3KeyPressed()
{
    if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(3);
}

void ACustomSpectatorPawn::On4KeyPressed()
{
    if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(4);
}

void ACustomSpectatorPawn::On0KeyPressed()
{
    if(EHPlayerController) EHPlayerController->DEBUG_Rebirth();
}

void ACustomSpectatorPawn::Client_UpdateActorLocation_Implementation(const FVector& NewLocation)
{
    // 서버의 위치를 강제로 반영 (아마.. 빙의중이라 이렇게안하면 위치 업데이트 적용안되는듯)
    SetActorLocation(NewLocation);
}
