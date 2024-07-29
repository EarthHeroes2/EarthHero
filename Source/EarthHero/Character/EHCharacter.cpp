#include "EHCharacter.h"

#include "AIController.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PostProcessComponent.h"
#include "EarthHero/Stat/StatComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../ForceField/BossZone.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "EarthHero/ForceField/DifficultyZone.h"
#include "DrawDebugHelpers.h"

AEHCharacter::AEHCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS Camera"));
    FPSCamera->SetupAttachment(RootComponent);
    FPSCamera->bUsePawnControlRotation = true;

    FirstPersonHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS Hand"));
    FirstPersonHand->SetupAttachment(FPSCamera);

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    WeaponMesh->SetupAttachment(FirstPersonHand, FName("FPS_RightHand"));

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(RootComponent);

    ForceFieldPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("ForceFieldPostProcessComponent"));
    ForceFieldPostProcessComponent->bEnabled = false;
    ForceFieldPostProcessComponent->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UMaterial> PostProcessMaterial(TEXT("/Game/Blueprints/HUD/PP_ForceField_Damage.PP_ForceField_Damage"));
    if (PostProcessMaterial.Succeeded())
    {
        ForceFieldPostProcessMaterial = PostProcessMaterial.Object;
        ForceFieldPostProcessComponent->Settings.AddBlendable(ForceFieldPostProcessMaterial, 1.0f);
    }

    BossZone = nullptr;
    bIsInBossZone = false;
    bIsInForceField = false;

    GetCharacterMovement()->MaxWalkSpeed = 500.f;

    SpawnRadius = 500.0f;
    SpawnInterval = 5.0f;
    bShowDebugCircle = true;
}

void AEHCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Controller)
    {
        FRotator NewRotator = Controller->GetControlRotation();
        float NewPitch;
        if (NewRotator.Pitch < 180.f)
        {
            NewPitch = FMath::Clamp(NewRotator.Pitch, MinPitchAngle, MaxPitchAngle);
        }
        else
        {
            NewPitch = FMath::Clamp(NewRotator.Pitch - 360.f, MinPitchAngle, MaxPitchAngle);
        }
        NewRotator.Pitch = NewPitch;
        Controller->SetControlRotation(NewRotator);
    }

    if (bShowDebugCircle)
    {
        DrawDebugCircle(GetWorld(), GetActorLocation(), SpawnRadius, 50, FColor::Green, false, -1.0f, 0, 5.0f, FVector(1, 0, 0), FVector(0, 1, 0), false);
    }
}

void AEHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AEHCharacter::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABP_BossZone::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        BossZone = Cast<ABP_BossZone>(FoundActors[0]);
    }

    Initialize();

    // Set a timer to call the wrapper function
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEHCharacter::SpawnActorsForDifficultyWrapper, SpawnInterval, true);
}

void AEHCharacter::SpawnActorsForDifficultyWrapper()
{
    SpawnActorsForDifficulty(AverageDifficulty);
}

void AEHCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    MyPlayerState = Cast<AEHPlayerState>(NewController->PlayerState);
    GetWorldTimerManager().SetTimer(SetStatComponentTimerHandle, this, &AEHCharacter::SetStatComponent, 2.f, true);

    APlayerController* NewPlayerController = Cast<APlayerController>(NewController);

    //Client_DisableAllInput(NewPlayerController); //이거 인게임 테스트 불가능해져서 일단 꺼둠 - 박정익
}

void AEHCharacter::Client_DisableAllInput_Implementation(APlayerController* PlayerController)
{
    PlayerController->DisableInput(PlayerController);
}

void AEHCharacter::SetStatComponent()
{
    // 승언 : EHPlayerState에서 StatComponent의 참조 가져오기
    if (MyPlayerState && MyPlayerState->IsSetStatComponentEnd)
    {
        GetWorldTimerManager().ClearTimer(SetStatComponentTimerHandle);
        StatComponent = MyPlayerState->GetStatComponent();
        if (StatComponent)
        {
            UE_LOG(LogTemp, Error, TEXT("EHCharacter.cpp: StatComponent Set Success"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("EHCharacter.cpp: StatComponent Set failed"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EHCharacter.cpp: Fail to Get StatComponent"));
    }
}

void AEHCharacter::Initialize()
{
    // First Person, Third Person Weapon Position Setting
    if (IsLocallyControlled())
    {
        WeaponMesh->AttachToComponent(FirstPersonHand, FAttachmentTransformRules::KeepRelativeTransform, FName("FPS_RightHand"));
    }
    else
    {
        WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("TPS_RightHand"));
    }
}

void AEHCharacter::EnableForceFieldEffect()
{
    if (IsLocallyControlled() && ForceFieldPostProcessComponent && !bIsInBossZone)
    {
        ForceFieldPostProcessComponent->bEnabled = true;
    }
}

void AEHCharacter::DisableForceFieldEffect()
{
    if (IsLocallyControlled() && ForceFieldPostProcessComponent)
    {
        ForceFieldPostProcessComponent->bEnabled = false;
    }
}

bool AEHCharacter::IsInBossZone() const
{
    return bIsInBossZone;
}

void AEHCharacter::SetIsInBossZone(bool bInBossZone)
{
    bIsInBossZone = bInBossZone;
    if (bIsInBossZone)
    {
        DisableForceFieldEffect();
    }
    else if (bIsInForceField)
    {
        EnableForceFieldEffect();
    }
}

bool AEHCharacter::IsInForceField() const
{
    return bIsInForceField;
}

void AEHCharacter::SetIsInForceField(bool bInForceField)
{
    bIsInForceField = bInForceField;
    if (bIsInForceField && !bIsInBossZone)
    {
        EnableForceFieldEffect();
    }
    else
    {
        DisableForceFieldEffect();
    }
}

void AEHCharacter::AddDifficultyZone(ADifficultyZone* Zone)
{
    OverlappingDifficultyZones.AddUnique(Zone);
    UpdateDifficulty();
}

void AEHCharacter::RemoveDifficultyZone(ADifficultyZone* Zone)
{
    OverlappingDifficultyZones.Remove(Zone);
    UpdateDifficulty();
}

void AEHCharacter::UpdateDifficulty()
{
    if (OverlappingDifficultyZones.Num() == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("No overlapping difficulty zones."));
        AverageDifficulty = 1.0f;
        return;
    }

    float TotalDifficulty = 0.0f;
    for (ADifficultyZone* Zone : OverlappingDifficultyZones)
    {
        TotalDifficulty += Zone->Difficulty;
    }

    AverageDifficulty = TotalDifficulty / OverlappingDifficultyZones.Num();
    UE_LOG(LogTemp, Log, TEXT("Updated difficulty to: %f"), AverageDifficulty);
}

void AEHCharacter::SpawnActorsForDifficulty(float Difficulty)
{
    TArray<TSubclassOf<AActor>> ActorClassesToSpawn;
    int32 TotalActorsToSpawn = 0;

    if (Difficulty == 1.0f)
    {
        ActorClassesToSpawn = Difficulty1Config.ActorClasses;
        TotalActorsToSpawn = 10;
    }
    else if (Difficulty == 2.0f)
    {
        ActorClassesToSpawn = Difficulty2Config.ActorClasses;
        TotalActorsToSpawn = 20;
    }
    else if (Difficulty == 3.0f)
    {
        ActorClassesToSpawn = Difficulty3Config.ActorClasses;
        TotalActorsToSpawn = 30;
    }
    else if (Difficulty == 1.5f)
    {
        ActorClassesToSpawn = Difficulty1Config.ActorClasses;
        ActorClassesToSpawn.Append(Difficulty2Config.ActorClasses);
        TotalActorsToSpawn = 15; // 5 from Difficulty 1 and 10 from Difficulty 2
    }
    else if (Difficulty == 2.5f)
    {
        ActorClassesToSpawn = Difficulty2Config.ActorClasses;
        ActorClassesToSpawn.Append(Difficulty3Config.ActorClasses);
        TotalActorsToSpawn = 25; // 10 from Difficulty 2 and 15 from Difficulty 3
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid difficulty level: %f"), Difficulty);
        return;
    }

    if (ActorClassesToSpawn.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No actors to spawn for difficulty level: %f"), Difficulty);
        return;
    }

    float AngleStep = 360.0f / TotalActorsToSpawn;
    FVector CharacterLocation = GetActorLocation();

    for (int32 i = 0; i < TotalActorsToSpawn; ++i)
    {
        float Angle = AngleStep * i;
        FVector SpawnLocation = CharacterLocation + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * SpawnRadius, FMath::Sin(FMath::DegreesToRadians(Angle)) * SpawnRadius, 0.0f);

        // Perform a line trace to find the ground level at the spawn location
        FVector StartLocation = SpawnLocation + FVector(0.0f, 0.0f, 100000.0f); // Start above the ground
        FVector EndLocation = SpawnLocation - FVector(0.0f, 0.0f, 200000.0f); // End below the ground

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        if (bHit)
        {
            // Set the Z value of the SpawnLocation to the hit location's Z value
            SpawnLocation.Z = HitResult.Location.Z;

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();
            
            TSubclassOf<AActor> ActorClassToSpawn = ActorClassesToSpawn[FMath::RandHelper(ActorClassesToSpawn.Num())];
            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClassToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
            
            if (SpawnedActor)
            {
                // If the spawned actor is a character, ensure it is possessed by an AI controller
                if (ACharacter* SpawnedCharacter = Cast<ACharacter>(SpawnedActor))
                {
                    AAIController* AIController = GetWorld()->SpawnActor<AAIController>(SpawnedCharacter->AIControllerClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
                    if (AIController)
                    {
                        AIController->Possess(SpawnedCharacter);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AI controller for %s"), *SpawnedCharacter->GetName());
                    }
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No hit detected for spawn location. Actor will not be spawned."));
        }
    }
}

void AEHCharacter::Shoot()
{
}

void AEHCharacter::Skill()
{
}
