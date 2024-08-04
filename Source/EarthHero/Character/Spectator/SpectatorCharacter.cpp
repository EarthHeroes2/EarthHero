// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASpectatorCharacter::ASpectatorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	SetReplicateMovement(true);

	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying); //이거 바로 falling으로 바뀜
	GetCharacterMovement()->AirControl = 1.0f;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASpectatorCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AController* MyController = GetController();
	if(MyController) EHPlayerController = Cast<AEHPlayerController>(MyController);
}

// Called every frame
void ASpectatorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASpectatorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ASpectatorCharacter::On1KeyPressed);
	PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ASpectatorCharacter::On2KeyPressed);
	PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ASpectatorCharacter::On3KeyPressed);
	PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ASpectatorCharacter::On4KeyPressed);
	PlayerInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &ASpectatorCharacter::On0KeyPressed);
}

void ASpectatorCharacter::On1KeyPressed()
{
	if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(1);
}

void ASpectatorCharacter::On2KeyPressed()
{
	if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(2);
}

void ASpectatorCharacter::On3KeyPressed()
{
	if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(3);
}

void ASpectatorCharacter::On4KeyPressed()
{
	if(EHPlayerController) EHPlayerController->Server_SpectatePlayer(4);
}

void ASpectatorCharacter::On0KeyPressed()
{
	if(EHPlayerController) EHPlayerController->DEBUG_Rebirth();
}