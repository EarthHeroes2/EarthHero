#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawningComponent.generated.h"

USTRUCT(BlueprintType)
struct FSpawnConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<TSubclassOf<AActor>> ActorClasses;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EARTHHERO_API USpawningComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USpawningComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnActorsForDifficulty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bShowDebugCircle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    FSpawnConfiguration Difficulty1Config;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    FSpawnConfiguration Difficulty2Config;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    FSpawnConfiguration Difficulty3Config;

private:
    FTimerHandle SpawnTimerHandle;

    void InitializeDifficultyActors();
    void SpawnActorsForDifficultyWrapper();

    // Add path constants here
    static const TCHAR* const Difficulty1Path;
    static const TCHAR* const Difficulty2Path1;
    static const TCHAR* const Difficulty2Path2;
    static const TCHAR* const Difficulty2Path3;
    static const TCHAR* const Difficulty2Path4;
    static const TCHAR* const Difficulty3Path;
};
