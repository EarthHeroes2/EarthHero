#include "PlayingGameMode.h"

#include "EarthHero/Character/Shooter/EHShooter.h"
#include "EarthHero/Character/Warrior/EHWarrior.h"
#include "EarthHero/Enum/Enums.h"
#include "EarthHero/ForceField/ForceField.h"
#include "EarthHero/GameSession/PlayingGameSession.h"
#include "EarthHero/GameState/PlayingGameState.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "Kismet/GameplayStatics.h"

APlayingGameMode::APlayingGameMode()
{

	CharacterClasses.SetNum(4);
	
	//Warrior
	static ConstructorHelpers::FClassFinder<AEHWarrior> EHWarriorAsset(TEXT("/Game/Blueprints/Character/Warrior/BP_Warrior.BP_Warrior_C"));
	if (EHWarriorAsset.Succeeded()) CharacterClasses[Warrior] = EHWarriorClass = EHWarriorAsset.Class;

	//Mechanic

	//shooter
	static ConstructorHelpers::FClassFinder<AEHShooter> EHShooterAsset(TEXT("/Game/Blueprints/Character/Shooter/BP_Shooter.BP_Shooter_C"));
	if (EHShooterAsset.Succeeded()) CharacterClasses[Shooter] = EHShooterClass = EHShooterAsset.Class;

	//Archor
	
	PrimaryActorTick.bCanEverTick = true;
}

void APlayingGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	bUseSeamlessTravel = true;
}

APawn* APlayingGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	UE_LOG(LogClass, Warning, TEXT("SpawnDefaultPawnFor_Implementation"));
	APlayerState* PlayerState = NewPlayer->PlayerState;
	if(PlayerState)
	{
		AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
		if(EHPlayerState)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			UWorld* World = GetWorld();
			if(World)
			{
				APawn* SpawnedCharacter = World->SpawnActor<APawn>(CharacterClasses[EHPlayerState->PlayerClass], StartSpot->GetActorTransform(), SpawnParams);
				if(SpawnedCharacter)
				{
					UE_LOG(LogClass, Warning, TEXT("SpawnDefaultPawnFor_Implementation Success!~~~~~"));
					return SpawnedCharacter;
				}
			}
		}
	}
	return nullptr;
}

void APlayingGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateWorldMapInfo();
}

void APlayingGameMode::UpdateWorldMapInfo()
{
	TArray<FVector2D> ActorLocations;
	TArray<float> ActorRotations;
	for (AActor* Actor : Players)
	{
		if (Actor)
		{
			FVector ActorLocation = Actor->GetActorLocation();
			FRotator ActorRotation = Actor->GetActorRotation();
			ActorLocations.Add(FVector2D(ActorLocation.X, ActorLocation.Y));
			ActorRotations.Add(ActorRotation.Yaw);
		}
		//UE_LOG(LogClass, Warning, TEXT("UpdateWorldMapInfo"));
	}
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateWorldMaps(ActorLocations, ActorRotations, Players.Num());
}

void APlayingGameMode::SpawnForceFields()
{
    // 아래 값들은 실제 맵 크기 넣으면 됨
    float MinX = 0.0f;
    float MaxX = 403200.0f;
    float MinY = -403200.0f;
    float MaxY = 0.0f;
    float Range = MaxX - MinX;

    // 맵 크기를 스케일 10으로 전환
    float Scale = 10.0f / Range;

    // 자기장끼리 거리 최소 3
    float SegmentMin = 3.0f / Scale;

    // 위쪽 자기장
    float InitialX = FMath::RandRange(MinX, MaxX);
    FVector2D TopLocation = FVector2D(InitialX, MaxY);
    ForceFieldLocations.Add(TopLocation);

    // 오른쪽 자기장
    float RightY = FMath::RandRange(MinY + SegmentMin, MaxY - SegmentMin);
    FVector2D RightLocation = FVector2D(MaxX, RightY);
    ForceFieldLocations.Add(RightLocation);

    // 아래 자기장
    float BottomX = FMath::RandRange(MinX + SegmentMin, MaxX - SegmentMin);
    FVector2D BottomLocation = FVector2D(BottomX, MinY);
    ForceFieldLocations.Add(BottomLocation);

    // 왼쪽 자기장
    float LeftY = FMath::RandRange(MinY + SegmentMin, MaxY - SegmentMin);
    FVector2D LeftLocation = FVector2D(MinX, LeftY);
    ForceFieldLocations.Add(LeftLocation);

    // 각 자기장 거리 3 >= 아니면 재조정
    while (!IsValidForceFieldDistance(ForceFieldLocations, SegmentMin))
    {
        ForceFieldLocations.Empty();
        
        InitialX = FMath::RandRange(MinX, MaxX);
        TopLocation = FVector2D(InitialX, MaxY);
        ForceFieldLocations.Add(TopLocation);

        RightY = FMath::RandRange(MinY + SegmentMin, MaxY - SegmentMin);
        RightLocation = FVector2D(MaxX, RightY);
        ForceFieldLocations.Add(RightLocation);

        BottomX = FMath::RandRange(MinX + SegmentMin, MaxX - SegmentMin);
        BottomLocation = FVector2D(BottomX, MinY);
        ForceFieldLocations.Add(BottomLocation);

        LeftY = FMath::RandRange(MinY + SegmentMin, MaxY - SegmentMin);
        LeftLocation = FVector2D(MinX, LeftY);
        ForceFieldLocations.Add(LeftLocation);
    }

    // 각 자기장 확장 시간 생성 (하나의 확장 시간은 900, 나머지는 600~900 사이)
    GenerateRandomDurations(4, 600.0f, 900.0f, ExpansionDurations);

    // 자기장 각 위치에 소환하기
    for (int i = 0; i < ForceFieldLocations.Num(); ++i)
    {
        SpawnForceFieldAtLocation(ForceFieldLocations[i], ExpansionDurations[i]); // Convert FVector2D to FVector
    }
}

// 거리 3 이상인지 확인
bool APlayingGameMode::IsValidForceFieldDistance(const TArray<FVector2D>& Locations, float MinDistance)
{
	for (int i = 0; i < Locations.Num(); ++i)
	{
		int NextIndex = (i + 1) % Locations.Num();
		float Distance = FVector2D::Distance(Locations[i], Locations[NextIndex]);

		if (Distance < MinDistance)
		{
			return false;
		}
	}

	return true;
}

void APlayingGameMode::GenerateRandomDurations(int Count, float Min, float Max, TArray<float>& OutDurations)
{
    // 하나의 확장 시간은 900으로 고정
    OutDurations.Add(900.0f);

    for (int i = 1; i < Count; ++i)
    {
        float RandomValue = FMath::RandRange(Min, Max);
        OutDurations.Add(RandomValue);
    }

    // 셔플 -> 랜덤화
    for (int i = 0; i < OutDurations.Num(); ++i)
    {
        int Index = FMath::RandRange(0, OutDurations.Num() - 1);
        OutDurations.Swap(i, Index);
    }
}

void APlayingGameMode::SpawnForceFieldAtLocation(FVector2D Location, float ExpansionDuration)
{
	if (ForceFieldActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ForceFieldActor class not set in GameMode"));
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		FRotator SpawnRotation = FRotator::ZeroRotator;
		FVector SpawnLocation = FVector(Location, 0.0f); // Convert FVector2D to FVector

		AActor* SpawnedForceField = World->SpawnActor<AActor>(ForceFieldActor, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedForceField)
		{
			AForceField* ForceField = Cast<AForceField>(SpawnedForceField);
			if (ForceField)
			{
				// Each force field gets a new curve created
				UCurveFloat* NewCurve = NewObject<UCurveFloat>();
				NewCurve->FloatCurve.AddKey(0.0f, 0.0f);
				NewCurve->FloatCurve.AddKey(ExpansionDuration, 1.0f);
				ForceField->SetCustomCurve(NewCurve);
				ForceField->SetExpansionDuration(ExpansionDuration);

				UE_LOG(LogTemp, Warning, TEXT("Spawned ForceField at location: %s with duration: %f"), *SpawnLocation.ToString(), ExpansionDuration);
			}
		}
	}
}

//심레스 트래블 이후 새로운 컨트롤러 생김
void APlayingGameMode::InitSeamlessTravelPlayer(AController* NewController) //아니면... GenericPlayerInitialization?
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	APlayerController* NewPlayerController = Cast<APlayerController>(NewController);

	if(NewPlayerController)
	{
		AEHPlayerController* NewEHPlayerController = Cast<AEHPlayerController>(NewController);
		if(NewEHPlayerController)
		{
			APlayingGameSession* PlayingGameSession = Cast<APlayingGameSession>(GameSession);
			if (PlayingGameSession)
			{
				EHPlayerControllers.Add(NewEHPlayerController);
				//세션 속 모든 플레이어가 레벨에 들어왔다면 레벨 초기 작업 시작
				if(EHPlayerControllers.Num() == PlayingGameSession->GetNumPlayersInSession())
				{
					InitLevelSetting();
				}
			}
		}
	}
}

void APlayingGameMode::InitLevelSetting()
{
	for(int i = 0; i < EHPlayerControllers.Num(); i++)
	{
		//플레이어 시작위치를 정해줄 수는 있는데.. 코드를 보면 보이지는 않지만 폰이 2개 생성되는 것 같음 (보류)
		//AActor* TargetPlayerStart = FindPlayerStart(EHPlayerControllers[i], FString::FromInt(i));
		//RestartPlayerAtPlayerStart(EHPlayerControllers[i], TargetPlayerStart);
	}
}

void APlayingGameMode::PlayerControllerReady() //조금 느리지만 안전하게 다 확인하고
{
	NumPlayerControllerReady++;
	
	//모든 플레이어가 준비 완료되었다면
	if(NumPlayerControllerReady == EHPlayerControllers.Num())
	{
		SpawnForceFields();

		//지도에 자기장 및 플레이어 표시를 위한 설정
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEHCharacter::StaticClass(), Players);
		APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
		PlayingGameState->SetGameStateForceField(ExpansionDurations, ForceFieldLocations);
		
		UpdateGameStateNames();
		UpdateGameStateClasses();
		UpdateGameStateExps();
		UpdateGameStateLevels();
		UpdateGameStateHealths();
		UpdateGameStateKillCount();
		UpdateGameStateDamage();
		UpdateGameStateReceiveDamage();
		UpdateGameStateHeal();
	 
		//모두의 움직임 풀어줌
		EnableAllInput();

		//게임 시간은 흐르기 시작
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ThisClass::GameTimerCount, 1.0f, true);
	}
}

void APlayingGameMode::EnableAllInput() //모든 플레이어 input 허용
{
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		EHPlayerController->Client_EnableInput();
	}
}

void APlayingGameMode::PlayerLogOut(const AEHPlayerController* ConstExitingEHPlayerController)
{
	AEHPlayerController* ExitingEHPlayerController = const_cast<AEHPlayerController*>(ConstExitingEHPlayerController);
	if(ExitingEHPlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Remove exit player information..."));
	
		int PlayerIndex = EHPlayerControllers.IndexOfByKey(ExitingEHPlayerController);
		
		if (PlayerIndex != INDEX_NONE) EHPlayerControllers.RemoveAt(PlayerIndex);

		//이러면 나간 자리는 초기화 해줘야겠네
		UpdateGameStateNames();
		UpdateGameStateClasses();
		UpdateGameStateExps();
		UpdateGameStateLevels();
		UpdateGameStateHealths();

		/*
		if(이미 죽은 플레이어) NumDeadPlayers--;
		CheckAllPlayerDead();
		*/
	}
}

void APlayingGameMode::SendChatMessage(const FText& Text)
{
	int32 NumberOfPlayers = EHPlayerControllers.Num();

	UE_LOG(LogTemp, Log, TEXT("Send a message to clients"));

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (EHPlayerControllers[i])
		{
			EHPlayerControllers[i]->Client_SendChatMessage(Text);
		}
	}
}

void APlayingGameMode::AddPlayerDead()
{
	NumDeadPlayers++;
	CheckAllPlayerDead();
}

void APlayingGameMode::CheckAllPlayerDead()
{
	if(NumDeadPlayers == EHPlayerControllers.Num())
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
		if(EHGameInstance)
		{
			EHGameInstance->bGameClear = false;
			GetWorld()->ServerTravel(GameOverMap, true);
		}
	}
}

void APlayingGameMode::BossDead()
{
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		EHGameInstance->bGameClear = true;
		GetWorld()->ServerTravel(GameOverMap, true);
	}
}


void APlayingGameMode::GameTimerCount()
{
	GameTimer++;

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateHUDGameTimer(GameTimer);
}

void APlayingGameMode::UpdateGameStateHealths()
{
	TArray<float> PlayerMaxHealths;
	TArray<float> PlayerCurrentHealths;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
				{
					PlayerMaxHealths.Add(StatComponent->GetMaxHealth());
					PlayerCurrentHealths.Add(StatComponent->GetHealth());
				}
			}
		}
	}
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateHealths(PlayerMaxHealths, PlayerCurrentHealths);
}

void APlayingGameMode::UpdateGameStateLevels()
{
	TArray<int> PlayerLevels;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerLevels.Add(StatComponent->GetLevel());
			}
		}
	}
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateLevels(PlayerLevels);
}

void APlayingGameMode::UpdateGameStateExps()
{
	TArray<float> PlayerExps;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerExps.Add(StatComponent->GetExpPercent());
			}
		}
	}
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateExps(PlayerExps);
}

void APlayingGameMode::UpdateGameStateNames()
{
	TArray<FString> PlayerNames;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			PlayerNames.Add(EHPlayerController->PlayerState->GetPlayerName());
		}
	}
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateNames(PlayerNames);
}

void APlayingGameMode::UpdateGameStateClasses()
{
	TArray<int> PlayerClasses;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
				PlayerClasses.Add(EHPlayerState->PlayerClass);
		}
	}
	
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateClasses(PlayerClasses);
}

void APlayingGameMode::UpdateGameStateKillCount()
{
	TArray<int> PlayerKillCount;
	
	for (AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerKillCount.Add(StatComponent->GetKillCount());
			}
		}
	}

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateKillCount(PlayerKillCount);
}

void APlayingGameMode::UpdateGameStateDamage()
{
	TArray<float> PlayerGivenDamage;
	
	for (AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerGivenDamage.Add(StatComponent->GetGivenDamage());
			}
		}
	}
	
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateGivenDamage(PlayerGivenDamage);
}

void APlayingGameMode::UpdateGameStateReceiveDamage()
{
	TArray<float> PlayerReceiveDamage;
	
	for (AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerReceiveDamage.Add(StatComponent->GetReceiveDamage());
			}
		}
	}
	
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateReceiveDamage(PlayerReceiveDamage);
}

void APlayingGameMode::UpdateGameStateHeal()
{
	TArray<float> PlayerHeal;
	
	for (AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerHeal.Add(StatComponent->GetHeal());
			}
		}
	}

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateHeal(PlayerHeal);
}

void APlayingGameMode::UpdatePlayerClassImage()
{
	TArray<UTexture2D*> ClassImage;
	static ConstructorHelpers::FObjectFinder<UTexture2D> WrTexture(TEXT("/Game/Assets/Textures/SwordImage.SwordImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> McTexture(TEXT("/Game/Assets/Textures/DroneImage.DroneImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> StTexture(TEXT("/Game/Assets/Textures/GunImage.GunImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> AcTexture(TEXT("/Game/Assets/Textures/BowImage.BowImage"));
	
	for (AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				switch (EHPlayerState->PlayerClass)
				{
				case Warrior:
						ClassImage.Add(WrTexture.Object);
				case Mechanic:
						ClassImage.Add(McTexture.Object);
				case Shooter:
						ClassImage.Add(StTexture.Object);
				case Archer:
						ClassImage.Add(AcTexture.Object);
				}
			}
		}
	}
	
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpatePlayerClassImage(ClassImage);
}

