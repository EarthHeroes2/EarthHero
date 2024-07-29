#include "EHCharacter.h"
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

    // Initialize the SpawningComponent
    SpawningComponent = CreateDefaultSubobject<USpawningComponent>(TEXT("SpawningComponent"));
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
}

void AEHCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    MyPlayerState = Cast<AEHPlayerState>(NewController->PlayerState);
    GetWorldTimerManager().SetTimer(SetStatComponentTimerHandle, this, &AEHCharacter::SetStatComponent, 2.f, true);

    APlayerController* NewPlayerController = Cast<APlayerController>(NewController);
}

void AEHCharacter::Client_DisableAllInput_Implementation(APlayerController* PlayerController)
{
    PlayerController->DisableInput(PlayerController);
}

void AEHCharacter::SetStatComponent()
{
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

void AEHCharacter::Shoot()
{
}

void AEHCharacter::Skill()
{
}
