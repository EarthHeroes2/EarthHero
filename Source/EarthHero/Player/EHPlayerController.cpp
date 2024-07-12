#include "EHPlayerController.h"

#include "EHPlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "EarthHero/HUD/TabHUDWidget.h"
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

	if (!IsRunningDedicatedServer())
	{
		check(HeroContext);
	}
}

//서버에서 빙의 알아챔
void AEHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ClientPossess();
}

//클라이언트에게 빙의됨을 알려줌
void AEHPlayerController::ClientPossess_Implementation()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(HeroContext, 0);
	}
	GetWorldTimerManager().SetTimer(PlayerStateCheckTimerHandle, this, &AEHPlayerController::InitializeHUD, 0.1f, true);
}

//여기서 HUD 설정
void AEHPlayerController::InitializeHUD()
{
	AEHPlayerState* MyPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (MyPlayerState && MyPlayerState->GetStatComponent())
	{
		GetWorldTimerManager().ClearTimer(PlayerStateCheckTimerHandle);

		HUD = Cast<UInGameHUD>(CreateWidget(this, InGameHUD));
		if (HUD)
		{
			HUD->InitializePlayerState(MyPlayerState->GetStatComponent());
			HUD->AddToViewport();
			MyPlayerState->GetStatComponent()->SetInGameHUD(HUD);
		}
		else UE_LOG(LogClass, Warning, TEXT("EHPlayerController(Client): HUD Cast Failed"));
		
		// Initialize and hide TabHUD
		TabHUD = CreateWidget<UTabHUDWidget>(this, TabHUDClass);
		if (TabHUD)
		{
			TabHUD->AddToViewport();
			TabHUD->SetVisibility(ESlateVisibility::Hidden);
			MyPlayerState->GetStatComponent()->SetTabHUD(TabHUD);
		}

		//준비가 다 되었으니 서버에게 이 사실을 알림
		Server_PlayerControllerReady();
	}
}

//서버에게 플레이어가 시작할 준비가 됨을 알림
void AEHPlayerController::Server_PlayerControllerReady_Implementation()
{
	APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayingGameMode)
	{
		PlayingGameMode->PlayerControllerReady();
	}
}

void AEHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ThisClass::Shoot);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &ThisClass::Skill);

	EnhancedInputComponent->BindAction(TabAction, ETriggerEvent::Started, this, &ThisClass::ShowTabHUD);
	EnhancedInputComponent->BindAction(TabAction, ETriggerEvent::Completed, this, &ThisClass::HideTabHUD);

	EnhancedInputComponent->BindAction(ChatAction, ETriggerEvent::Triggered, this, &ThisClass::FocusChatBox);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AEHPlayerController::Jump()
{
	if (GetPawn())
	{
		Cast<AEHCharacter>(GetPawn())->Jump();
	}
}

void AEHPlayerController::Shoot()
{
	if (GetPawn())
	{
		Cast<AEHCharacter>(GetPawn())->Shoot();
	}
}

void AEHPlayerController::Skill()
{
	if (GetPawn())
	{
		Cast<AEHCharacter>(GetPawn())->Skill();
	}
}

void AEHPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (GetPawn())
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

void AEHPlayerController::ShowTabHUD()
{
	if (TabHUD)
	{
		TabHUD->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEHPlayerController::HideTabHUD()
{
	if (TabHUD)
	{
		TabHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AEHPlayerController::FocusChatBox()
{
	if (HUD)
	{
		UEditableTextBox* Chat = HUD->Chat_Etb;
		if(!Chat->HasKeyboardFocus()) Chat->SetKeyboardFocus();
	}
}





void AEHPlayerController::Client_EnableInput_Implementation()
{
	EnableInput(this);
}







void AEHPlayerController::Server_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Send a chat message"));

	FText PlayerName = FText::FromString(PlayerState->GetPlayerName());
	FText ChatText = FText::Format(FText::FromString("[{0}] : {1}"), PlayerName, Text);

	APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayingGameMode)
	{
		PlayingGameMode->SendChatMessage(ChatText);
	}
}

void AEHPlayerController::Client_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Receive a chat message"));
	if (HUD)
	{
		HUD->AddChatMessage(Text);
	}
}