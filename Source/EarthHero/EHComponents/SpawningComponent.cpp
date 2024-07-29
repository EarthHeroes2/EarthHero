#include "SpawningComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "EarthHero/Character/EHCharacter.h"

const TCHAR* const USpawningComponent::Difficulty1Path = TEXT("Actor'/Game/Blueprints/Item/BP_ExpItem.BP_ExpItem_C'");
const TCHAR* const USpawningComponent::Difficulty2Path1 = TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_DummyRangeMonster.BP_DummyRangeMonster_C'");
const TCHAR* const USpawningComponent::Difficulty2Path2 = TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_DummyMeleeMonster.BP_DummyMeleeMonster_C'");
const TCHAR* const USpawningComponent::Difficulty2Path3 = TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_DummyFlyingRangeMonster.BP_DummyFlyingRangeMonster_C'");
const TCHAR* const USpawningComponent::Difficulty2Path4 = TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_DummyFlyingMeleeMonster.BP_DummyFlyingMeleeMonster_C'");
const TCHAR* const USpawningComponent::Difficulty3Path = TEXT("Actor'/Game/Blueprints/Character/MidBoss/BP_MidBoss1Monster.BP_MidBoss1Monster_C'");

USpawningComponent::USpawningComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    SpawnRadius = 5000.0f;
    SpawnInterval = 10.0f;
    bShowDebugCircle = true;

    InitializeDifficultyActors();
}

void USpawningComponent::InitializeDifficultyActors()
{
    // Load difficulty 1 actors
    static ConstructorHelpers::FClassFinder<AActor> Difficulty1ActorClass(Difficulty1Path);
    if (Difficulty1ActorClass.Succeeded())
    {
        Difficulty1Config.ActorClasses.Add(Difficulty1ActorClass.Class);
    }

    // Load difficulty 2 actors
    static ConstructorHelpers::FClassFinder<AActor> Difficulty2ActorClass1(Difficulty2Path1);
    if (Difficulty2ActorClass1.Succeeded())
    {
        Difficulty2Config.ActorClasses.Add(Difficulty2ActorClass1.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty2ActorClass2(Difficulty2Path2);
    if (Difficulty2ActorClass2.Succeeded())
    {
        Difficulty2Config.ActorClasses.Add(Difficulty2ActorClass2.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty2ActorClass3(Difficulty2Path3);
    if (Difficulty2ActorClass3.Succeeded())
    {
        Difficulty2Config.ActorClasses.Add(Difficulty2ActorClass3.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty2ActorClass4(Difficulty2Path4);
    if (Difficulty2ActorClass4.Succeeded())
    {
        Difficulty2Config.ActorClasses.Add(Difficulty2ActorClass4.Class);
    }

    // Load difficulty 3 actors
    static ConstructorHelpers::FClassFinder<AActor> Difficulty3ActorClass(Difficulty3Path);
    if (Difficulty3ActorClass.Succeeded())
    {
        Difficulty3Config.ActorClasses.Add(Difficulty3ActorClass.Class);
    }
}

void USpawningComponent::BeginPlay()
{
    Super::BeginPlay();

    // Set a timer to call the wrapper function
    GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &USpawningComponent::SpawnActorsForDifficultyWrapper, SpawnInterval, true);
}

void USpawningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bShowDebugCircle)
    {
        DrawDebugCircle(GetWorld(), GetOwner()->GetActorLocation(), SpawnRadius, 50, FColor::Green, false, -1.0f, 0, 5.0f, FVector(1, 0, 0), FVector(0, 1, 0), false);
    }
}

void USpawningComponent::SpawnActorsForDifficultyWrapper()
{
    SpawnActorsForDifficulty();
}

void USpawningComponent::SpawnActorsForDifficulty()
{
    AActor* Owner = GetOwner();
    AEHCharacter* OwnerCharacter = Cast<AEHCharacter>(Owner);
    if (!OwnerCharacter) return;

    float Difficulty = OwnerCharacter->AverageDifficulty;

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
    FVector CharacterLocation = GetOwner()->GetActorLocation();

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
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

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
