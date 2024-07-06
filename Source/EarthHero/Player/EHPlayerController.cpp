// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerController.h"

#include "EHPlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "GameFramework/Character.h"

AEHPlayerController::AEHPlayerController()
{
}

void AEHPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AEHPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(HeroContext);
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(HeroContext, 0);
	}
}

//승언 : 컨트롤러가 빙의했을 때
void AEHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ClientPossess();
}

void AEHPlayerController::ClientPossess_Implementation()
{
	//Client가 PlayerState를 전달받는 게 느리기 때문에 받을 때까지 반복 호출한다.
	GetWorldTimerManager().SetTimer(PlayerStateCheckTimerHandle, this, &AEHPlayerController::InitializeHUD, 0.1f, true);
}

void AEHPlayerController::InitializeHUD()
{
	AEHPlayerState* MyPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (MyPlayerState && MyPlayerState->GetStatComponent())
	{
		GetWorldTimerManager().ClearTimer(PlayerStateCheckTimerHandle); // 타이머 해제

		// InGameHUD를 Viewport에 추가
		HUD = Cast<UInGameHUD>(CreateWidget(this, InGameHUD));
		if (HUD)
		{
			//MyPlayerState(리플리케이트)->StatComponent->HUD로 전달
			HUD->InitializePlayerState(MyPlayerState->GetStatComponent());
			HUD->AddToViewport();
			MyPlayerState->GetStatComponent()->SetInGameHUD(HUD);
		}
	}
}

void AEHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// PlayerController에 존재하는 InputComponent를 EnhancedInputComponent로 변환한다. 실패시 Error
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	// Shoot
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ThisClass::Shoot);
}

void AEHPlayerController::Jump()
{
	if(GetPawn())
	{
		Cast<AEHCharacter>(GetPawn())->Jump();
	}
}

void AEHPlayerController::Shoot()
{
	if(GetPawn())
	{
		Cast<AEHCharacter>(GetPawn())->Shoot();
	}
}

void AEHPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// GetControlRotation()으로 사용자의 Rotation 가져온다
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// Get Forward Vector
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get Right Vector
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Movement 추가
	if(GetPawn())
	{
		GetPawn()->AddMovementInput(ForwardVector, MovementVector.Y);
		GetPawn()->AddMovementInput(RightVector, MovementVector.X);
	}
}

void AEHPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>() * 0.5f;

	AddYawInput(LookVector.X);
	AddPitchInput(-LookVector.Y);
}
