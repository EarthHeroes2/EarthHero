// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpectatorCharacter.generated.h"

class AEHPlayerController;

UCLASS()
class EARTHHERO_API ASpectatorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpectatorCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void On1KeyPressed();
	void On2KeyPressed();
	void On3KeyPressed();
	void On4KeyPressed();
	void On0KeyPressed();

private:
	AEHPlayerController* EHPlayerController;

};
