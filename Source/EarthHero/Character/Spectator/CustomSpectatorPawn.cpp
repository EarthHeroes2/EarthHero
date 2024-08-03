// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSpectatorPawn.h"

#include "EarthHero/Player/EHPlayerController.h"

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
