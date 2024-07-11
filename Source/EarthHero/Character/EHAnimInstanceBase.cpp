// Fill out your copyright notice in the Description page of Project Settings.


#include "EHAnimInstanceBase.h"

#include "EHCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEHAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AEHCharacter>(TryGetPawnOwner());
}

void UEHAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Character == nullptr)
	{
		Character = Cast<AEHCharacter>(TryGetPawnOwner());
	}
	if(!Character) return;

	// Set Equipped Weapon
	EquippedWeapon = Character->GetEquippedWeapon();
	
	// Set Speed
	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	// Set Is In Air
	bIsInAir = Character->GetCharacterMovement()->IsFalling();

	// Set Is Accelerating
	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	// Calculate Rotations
	FRotator AimRotation = Character->GetBaseAimRotation();
	FRotator ActorRotation = Character->GetActorRotation();
	FRotator VelocityRotation = UKismetMathLibrary::MakeRotFromX(Velocity);

	// Set Direction
	FRotator DirectionDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, VelocityRotation);
	Direction = DirectionDeltaRotation.Yaw * (-1.f);

	// Set Offsets
	FRotator AimDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	YawOffset = AimDeltaRotation.Yaw;
	PitchOffset = AimDeltaRotation.Pitch;
	RollOffset = AimDeltaRotation.Roll;
}
