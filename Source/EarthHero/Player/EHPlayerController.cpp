#include "EHPlayerController.h"

#include "EHPlayerState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "EarthHero/CustomGameViewportClient.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Spectator/CustomSpectatorPawn.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "EarthHero/HUD/TabHUDWidget.h"
#include "EarthHero/HUD/EscMenu.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"


AEHPlayerController::AEHPlayerController()
{
}

void AEHPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEHPlayerController::Client_AddEffect_Implementation(UTexture2D* EffectImage, int ServerEffectType, float Duration)
{
	if (HUD)
	{
		HUD->AddStatusImage(EffectImage, ServerEffectType, Duration);
	}
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

	ACharacter* PossessCharacter = GetCharacter();
	if(PossessCharacter)
	{
		AEHCharacter* PossessEHCharacter = Cast<AEHCharacter>(PossessCharacter);
		if(PossessEHCharacter)
		{
			if(ControlledCharacter) //부활 때라면
			{
				UE_LOG(LogTemp, Log, TEXT("OnPossess() : rebirth"));
				Client_Possess(false);
			}
			else //최초 빙의 (겜 시작 때)라면
			{
				UE_LOG(LogTemp, Log, TEXT("OnPossess() : init"));
				ControlledCharacter = PossessCharacter;
				Client_Possess(true);
			}
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("OnPossess() : spectator"));
			Client_StartSpectate();
		}
	}
	else UE_LOG(LogClass, Error, TEXT("AEHPlayerController::OnPossess. PossessCharacter invalid"));

}

//빙의 풀림 (죽은 경우)
void AEHPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	UE_LOG(LogClass, Warning, TEXT("AEHPlayerController::OnUnPossess()"));
	
	if(DeadCharacter)
	{
		DeadCharacter = nullptr;
		
		UE_LOG(LogClass, Warning, TEXT("DeadLocation = %s, rotation = %s"), *DeadLocation.ToString(), *DeadLocation.Rotation().ToString());

		APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
		if (PlayingGameMode) PlayingGameMode->AddPlayerDead(this, DeadLocation);
	}
	else UE_LOG(LogClass, Warning, TEXT("Unpossess Spectator"));
}

//클라이언트에게 캐릭터에 빙의됨을 알려줌
void AEHPlayerController::Client_Possess_Implementation(bool bInit)
{
	if(bInit) //겜 시작 때 로직
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			if(!bInit) Subsystem->RemoveMappingContext(SpectatorContext);
			Subsystem->AddMappingContext(HeroContext, 0);
		}
		
		GetWorldTimerManager().SetTimer(PlayerStateCheckTimerHandle, this, &AEHPlayerController::InitializeHUD, 0.1f, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("EnableInput"));
	EnableInput(this);
}

//여기서 HUD 설정
void AEHPlayerController::InitializeHUD()
{
	MyPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (MyPlayerState && MyPlayerState->GetStatComponent() && MyPlayerState->GetHeroUpgradeComponent() && MyPlayerState->IsSetStatComponentEnd)
	{
		UE_LOG(LogClass, Warning, TEXT("EHPlayerController(Client): HUD 설정 시작"));
		GetWorldTimerManager().ClearTimer(PlayerStateCheckTimerHandle);

		HUD = Cast<UInGameHUD>(CreateWidget(this, InGameHUD));
		if (HUD)
		{
			HUD->InitializePlayerState(MyPlayerState->GetStatComponent());
			HUD->AddToViewport();
			MyPlayerState->GetStatComponent()->SetInGameHUD(HUD);
			MyPlayerState->GetHeroUpgradeComponent()->SetInGameHUD(HUD);
		}
		else UE_LOG(LogClass, Warning, TEXT("EHPlayerController(Client): HUD Cast Failed"));
		
		// Initialize and hide TabHUD
		TabHUD = CreateWidget<UTabHUDWidget>(this, TabHUDClass);
		if (TabHUD)
		{
			TabHUD->AddToViewport();
			TabHUD->SetVisibility(ESlateVisibility::Hidden);
			MyPlayerState->GetStatComponent()->SetTabHUD(TabHUD);
			MyPlayerState->GetHeroUpgradeComponent()->SetTabHUD(TabHUD);
		}
		
		EscMenu = CreateWidget<UEscMenu>(this, EscMenuClass);
		if (EscMenu)
		{
			EscMenu->AddToViewport();
			EscMenu->SetVisibility(ESlateVisibility::Hidden);
		}
		
		bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

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
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ThisClass::Dash);

	EnhancedInputComponent->BindAction(TabAction, ETriggerEvent::Started, this, &ThisClass::ShowTabHUD);
	EnhancedInputComponent->BindAction(TabAction, ETriggerEvent::Completed, this, &ThisClass::HideTabHUD);

	EnhancedInputComponent->BindAction(ChatAction, ETriggerEvent::Triggered, this, &ThisClass::FocusChatBox);

	EnhancedInputComponent->BindAction(SelectHUAction_1, ETriggerEvent::Started, this, &ThisClass::SelectHU_1);
	EnhancedInputComponent->BindAction(SelectHUAction_2, ETriggerEvent::Started, this, &ThisClass::SelectHU_2);
	EnhancedInputComponent->BindAction(SelectHUAction_3, ETriggerEvent::Started, this, &ThisClass::SelectHU_3);

	EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Started, this, &ThisClass::ToggleEscMenu);
	
	EnhancedInputComponent->BindAction(DEBUG_LevelUp, ETriggerEvent::Triggered, this, &ThisClass::DEBUG_Levelup);
	EnhancedInputComponent->BindAction(DEBUG_DieKey, ETriggerEvent::Started, this, &ThisClass::DEBUG_Die);
}


void AEHPlayerController::ToggleEscMenu()
{
	if (EscMenu)
	{
		bool bIsVisible = EscMenu->IsVisible();
		EscMenu->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

		if (!bIsVisible)
		{
			// UI mode, 마우스 보이게 하기
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(EscMenu->TakeWidget());
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
		else
		{
			// Game mode
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = false;
		}
	}
	else
	{
		EscMenu = CreateWidget<UEscMenu>(this, EscMenuClass);
		if (EscMenu)
		{
			EscMenu->AddToViewport();
			EscMenu->SetVisibility(ESlateVisibility::Visible);

			// UI mode, 마우스 보이게 하기
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(EscMenu->TakeWidget());
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void AEHPlayerController::Jump()
{
	ACharacter* PossessedCharacter = GetCharacter();
	if(PossessedCharacter)
	{
		AEHCharacter* PossessedEHCharacter = Cast<AEHCharacter>(PossessedCharacter);
		PossessedEHCharacter->Jump();
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

void AEHPlayerController::Dash()
{
	if(GetPawn())
	{
		Cast<AEHCharacter>(GetPawn())->Dash();
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
	auto GameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		const FVector2D LookVector = Value.Get<FVector2D>() * GameInstance->MouseSensitivity;

		AddYawInput(LookVector.X);
		AddPitchInput(-LookVector.Y);
	}
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

void AEHPlayerController::SelectHU_1()
{
	if (HUD->IsHeroUpgradeReadys())
	{
		//서버에서 업그레이드 적용
		MyPlayerState->HeroUpgradeComponent->ApplyHeroUpgrade(0);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("IsHeroUpgradeReadys = false"));
	}
}

void AEHPlayerController::SelectHU_2()
{
	if (HUD->IsHeroUpgradeReadys())
	{
		MyPlayerState->HeroUpgradeComponent->ApplyHeroUpgrade(1);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("IsHeroUpgradeReadys = false"));
	}
}

void AEHPlayerController::SelectHU_3()
{
	if (HUD->IsHeroUpgradeReadys())
	{
		MyPlayerState->HeroUpgradeComponent->ApplyHeroUpgrade(2);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("IsHeroUpgradeReadys = false"));
	}
}

//클라이언트들이 전부 완료가 되면 해야할 것들
void AEHPlayerController::Client_GameStart_Implementation()
{
	//페이드 인
	const UWorld* World = GetWorld();
	if (World)
	{
		UCustomGameViewportClient* GameViewportClient = Cast<UCustomGameViewportClient>(World->GetGameViewport());
		if (GameViewportClient) GameViewportClient->Fade(1.5, false);
	}
	
	//로딩화면 제거
	UEHGameInstance* EHGameinstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameinstance) EHGameinstance->RemoveSeamlessLoadingScreen();
	
	EnableInput(this);
}

void AEHPlayerController::Server_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Send a chat message"));

	FText PlayerName = FText::FromString(PlayerState->GetPlayerName());
	FText ChatText = FText::Format(FText::FromString("[{0}]: {1}"), PlayerName, Text);

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

void AEHPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEHPlayerController::Client_GameOver_Implementation()
{
	UEHGameInstance* EHGameinstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameinstance) EHGameinstance->ShowSeamlessLoadingScreen();
	
	if (HUD) HUD->RemoveFromParent();
	else if(TabHUD) TabHUD->RemoveFromParent();
}


void AEHPlayerController::DEBUG_Die()
{
	Server_DEBUG_Die();
}

void AEHPlayerController::Server_DEBUG_Die_Implementation()
{
	AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (EHPlayerState)
	{
		FHitResult a;
		EHPlayerState->GetStatComponent()->DamageTaken(50, nullptr, a, nullptr, nullptr);
	}
}

void AEHPlayerController::DEBUG_Levelup()
{
	Server_DEBUG_Levelup();
}

void AEHPlayerController::Server_DEBUG_Levelup_Implementation()
{
	AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (EHPlayerState)
	{
		EHPlayerState->GetStatComponent()->UpdateExp(100);
	}
}

//죽은 경우 (서버에서 실행됨)
void AEHPlayerController::Dead()
{
	DeadCharacter = GetCharacter();
	if(DeadCharacter)
	{
		Client_DisableInput();
		DeadLocation = DeadCharacter->GetActorLocation();
		UnPossess();
	}
}

void AEHPlayerController::Client_DisableInput_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("DisableInput"));
	DisableInput(this);
}

//죽고 관전폰에 빙의한 뒤 불리는 함수
void AEHPlayerController::Client_StartSpectate_Implementation()
{
	bSpectating = true;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(HeroContext);
		Subsystem->AddMappingContext(SpectatorContext, 0);
	}

	UE_LOG(LogTemp, Warning, TEXT("StartSpectate. EnableInput"));
	EnableInput(this);
}


void AEHPlayerController::DEBUG_Rebirth()
{
	UE_LOG(LogTemp, Log, TEXT("DEBUG_Rebirth()"));
	
	Server_DEBUG_Rebirth();
}

void AEHPlayerController::Server_DEBUG_Rebirth_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("AEHPlayerController::Server_DEBUG_Rebirth_Implementation()"));
	APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayingGameMode)
	{
		PlayingGameMode->Rebirth(this);
	}
}

//서버에서만 실행되어야 함
void AEHPlayerController::Rebirth()
{
	UE_LOG(LogTemp, Log, TEXT("AEHPlayerController::Rebirth()"));

	APawn* ControlledPawn = GetPawn();
	if(ControlledPawn)
	{
		ACustomSpectatorPawn* CustomSpectatorPawn = Cast<ACustomSpectatorPawn>(ControlledPawn);
		if(CustomSpectatorPawn)
		{
			UnPossess();
			CustomSpectatorPawn->Destroy();
			Possess(ControlledCharacter);
		}
	}
}

//서버에게 해당 플레이어 위치를 물어보고 관람해도 된다면 해당 위치로 이동시킴
void AEHPlayerController::Server_SpectatePlayer_Implementation(int PlayerNumber)
{
	UE_LOG(LogTemp, Log, TEXT("Server_SpectatePlayer_Implementation(int PlayerNumber = %d)"), PlayerNumber);
	
	APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if(PlayingGameMode)
	{
		FVector PlayerLocation;
		if(PlayingGameMode->GetPlayerLocation(PlayerNumber, PlayerLocation))
		{
			UE_LOG(LogTemp, Log, TEXT("Player %d Location : %s"), PlayerNumber, *PlayerLocation.ToString());
			APawn* ControlledPawn = GetPawn();
			if(ControlledPawn)
			{
				ACustomSpectatorPawn* CustomSpectatorPawn = Cast<ACustomSpectatorPawn>(ControlledPawn);
				if(CustomSpectatorPawn)
				{
					if(CustomSpectatorPawn->SetActorLocation(PlayerLocation, false, nullptr, ETeleportType::TeleportPhysics))
					{
						UE_LOG(LogTemp, Log, TEXT("SetActorLocation Seccess"));

						CustomSpectatorPawn->Client_UpdateActorLocation(PlayerLocation);
					}
					else UE_LOG(LogTemp, Log, TEXT("SetActorLocation Failed"));
				}
			}
		}
		else UE_LOG(LogTemp, Log, TEXT("GetPlayerLocation Failed"));
	}
}