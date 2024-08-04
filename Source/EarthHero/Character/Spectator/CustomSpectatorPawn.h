// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "CustomSpectatorPawn.generated.h"

class UCameraComponent;
class AEHPlayerController;
/**
 * 
 */
UCLASS()
class EARTHHERO_API ACustomSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
	ACustomSpectatorPawn();
	virtual void Tick(float DeltaTime) override;

	UCameraComponent* CameraComp;

	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;
	virtual void BeginPlay() override;
	void On1KeyPressed();
	void On2KeyPressed();
	void On3KeyPressed();
	void On4KeyPressed();

	void On0KeyPressed();

	AEHPlayerController* EHPlayerController;

public:
	UFUNCTION(Client, Reliable)
	void Client_UpdateActorLocation(const FVector& NewLocation);
};