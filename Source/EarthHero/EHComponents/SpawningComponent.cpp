#include "SpawningComponent.h"
#include "DrawDebugHelpers.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

const TCHAR* const USpawningComponent::Difficulty2Path1 = TEXT("Actor'/Game/Blueprints/Character/Monster/Normal/BP_GroundRangeMonster.BP_GroundRangeMonster_C'");
const TCHAR* const USpawningComponent::Difficulty2Path2 = TEXT("Actor'/Game/Blueprints/Character/Monster/Normal/BP_GroundMeleeMonster.BP_GroundMeleeMonster_C'");

const TCHAR* const USpawningComponent::Difficulty2Path3 = TEXT("Actor'/Game/Blueprints/Character/Monster/Normal/BP_FlyingMeleeMonster.BP_FlyingMeleeMonster_C'");
const TCHAR* const USpawningComponent::Difficulty2Path4 = TEXT("Actor'/Game/Blueprints/Character/Monster/Normal/BP_FlyingMeleeMonster.BP_FlyingMeleeMonster_C'");
//const TCHAR* const USpawningComponent::Difficulty3Path = TEXT("Actor'/Game/Blueprints/Character/MidBoss/BP_MidBoss1Monster.BP_MidBoss1Monster_C'");
// 이거 미완성이라 꺼내면 안돼, fatal error 남. - 박정익

USpawningComponent::USpawningComponent()
{
    SpawnRadius = 5000.0f;
    SpawnInterval = 10.0f;
    bShowDebugCircle = true;
    MaxSpawnedActors = 10; // 인당 최대 몬스터 수
    CurrentSpawnedActors = 0;
    
    PrimaryComponentTick.bCanEverTick = true;

    if(GetNetMode() != NM_Client)
    {
        InitializeDifficultyActors();
    }
}

void USpawningComponent::InitializeDifficultyActors()
{
    // Load difficulty 1 actors
    static ConstructorHelpers::FClassFinder<AActor> Difficulty1ActorClass1(Difficulty2Path1);
    if (Difficulty1ActorClass1.Succeeded())
    {
        Difficulty1Config.ActorClasses.Add(Difficulty1ActorClass1.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty1ActorClass2(Difficulty2Path2);
    if (Difficulty1ActorClass2.Succeeded())
    {
        Difficulty1Config.ActorClasses.Add(Difficulty1ActorClass2.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty1ActorClass3(Difficulty2Path3);
    if (Difficulty1ActorClass3.Succeeded())
    {
        Difficulty1Config.ActorClasses.Add(Difficulty1ActorClass3.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty1ActorClass4(Difficulty2Path4);
    if (Difficulty1ActorClass4.Succeeded())
    {
        Difficulty1Config.ActorClasses.Add(Difficulty1ActorClass4.Class);
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
    static ConstructorHelpers::FClassFinder<AActor> Difficulty3ActorClass1(Difficulty2Path1);
    if (Difficulty3ActorClass1.Succeeded())
    {
        Difficulty3Config.ActorClasses.Add(Difficulty3ActorClass1.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty3ActorClass2(Difficulty2Path2);
    if (Difficulty3ActorClass2.Succeeded())
    {
        Difficulty3Config.ActorClasses.Add(Difficulty3ActorClass2.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty3ActorClass3(Difficulty2Path3);
    if (Difficulty3ActorClass3.Succeeded())
    {
        Difficulty3Config.ActorClasses.Add(Difficulty3ActorClass3.Class);
    }
    static ConstructorHelpers::FClassFinder<AActor> Difficulty3ActorClass4(Difficulty2Path4);
    if (Difficulty3ActorClass4.Succeeded())
    {
        Difficulty3Config.ActorClasses.Add(Difficulty3ActorClass4.Class);
    }
}

void USpawningComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if(GetNetMode() != NM_Client) //서버에서만 소환해야함 - 박정익
    {
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &USpawningComponent::SpawnActorsForDifficultyWrapper, SpawnInterval, true);
    }
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
        TotalActorsToSpawn = 15;
    }
    else if (Difficulty == 2.5f)
    {
        ActorClassesToSpawn = Difficulty2Config.ActorClasses;
        ActorClassesToSpawn.Append(Difficulty3Config.ActorClasses);
        TotalActorsToSpawn = 25;
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

    for (int32 i = 0; i < TotalActorsToSpawn && CurrentSpawnedActors < MaxSpawnedActors; ++i)
    {
        float Angle = AngleStep * i;
        FVector SpawnLocation = CharacterLocation + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * SpawnRadius, FMath::Sin(FMath::DegreesToRadians(Angle)) * SpawnRadius, 0.0f);

        FVector StartLocation = SpawnLocation + FVector(0.0f, 0.0f, 100000.0f);
        FVector EndLocation = SpawnLocation - FVector(0.0f, 0.0f, 200000.0f);

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        if (bHit)
        {
            SpawnLocation.Z = HitResult.Location.Z + 50.f;

            UWorld* World = GetWorld();
            if(World == nullptr) return;

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            TSubclassOf<AActor> ActorClassToSpawn = ActorClassesToSpawn[FMath::RandHelper(ActorClassesToSpawn.Num())];

            if(OwnerCharacter)
            {
                AActor* SpawnedActor = World->SpawnActor<AActor>(ActorClassToSpawn, SpawnLocation, OwnerCharacter->GetActorLocation().Rotation(), SpawnParams);
                if (SpawnedActor)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Spanw = %s"), *SpawnedActor->GetName());
                    
                    if (AMonsterBase* SpawnedMonsterBase = Cast<AMonsterBase>(SpawnedActor))
                    {
                        TSubclassOf<AController> AIControllerClass = SpawnedMonsterBase->AIControllerClass;
                        if(AIControllerClass)
                        {
                            AAIControllerBase* AIControllerBase = World->SpawnActor<AAIControllerBase>(AIControllerClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
                            if(AIControllerBase)
                            {
                                AIControllerBase->SetTargetPlayer(OwnerCharacter);
                                AIControllerBase->Possess(SpawnedMonsterBase);
                                ++CurrentSpawnedActors;
                                continue;
                            }
                            AIControllerBase->Destroy();
                        }
                    }
                }
                SpawnedActor->Destroy();
            }
        }
        else UE_LOG(LogTemp, Warning, TEXT("No hit detected for spawn location. Actor will not be spawned."));
    }

    if (CurrentSpawnedActors >= MaxSpawnedActors)
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
}
