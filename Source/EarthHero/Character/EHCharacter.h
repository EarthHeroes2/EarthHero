#pragma once

#include "CoreMinimal.h"
#include "EHCharacterBase.h"
#include "Components/PostProcessComponent.h"
#include "EarthHero/ForceField/BossZone.h"
#include "EHCharacter.generated.h"

class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
class ADifficultyZone;

USTRUCT(BlueprintType)
struct FSpawnConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TArray<TSubclassOf<AActor>> ActorClasses;
};

UCLASS()
class EARTHHERO_API AEHCharacter : public AEHCharacterBase
{
    GENERATED_BODY()
    friend class AEHPlayerController;

public:
    AEHCharacter();
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void EnableForceFieldEffect();
    void DisableForceFieldEffect();

    bool IsInBossZone() const;
    void SetIsInBossZone(bool bInBossZone);

    bool IsInForceField() const;
    void SetIsInForceField(bool bInForceField);

    void AddDifficultyZone(ADifficultyZone* Zone);
    void RemoveDifficultyZone(ADifficultyZone* Zone);
    void UpdateDifficulty();

    virtual void Shoot();
    virtual void Skill();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    class UStatComponent *StatComponent;

protected:
    virtual void BeginPlay() override;

    virtual void PossessedBy(AController* NewController) override;

    void Initialize();

    void SpawnActorsForDifficulty(float Difficulty);

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnRadius;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowDebugCircle;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FSpawnConfiguration Difficulty1Config;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FSpawnConfiguration Difficulty2Config;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FSpawnConfiguration Difficulty3Config;

private:
    FTimerHandle SetStatComponentTimerHandle;
    FTimerHandle SpawnTimerHandle;

    void SetStatComponent();

    UPROPERTY()
    class AEHPlayerState* MyPlayerState;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Camera")
    UCameraComponent* FPSCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Mesh")
    USkeletalMeshComponent* FirstPersonHand;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    UWidgetComponent* OverheadWidget;

    UPROPERTY(EditAnywhere, Category = "ForceField")
    UPostProcessComponent* ForceFieldPostProcessComponent;

    UPROPERTY()
    UMaterialInterface* ForceFieldPostProcessMaterial;

    UPROPERTY()
    ABP_BossZone* BossZone;

    float MaxPitchAngle = 0.f;
    float MinPitchAngle = 0.f;
    
    UPROPERTY()
    bool bIsInBossZone;
    UPROPERTY()
    bool bIsInForceField;
    
    float AverageDifficulty = 1.0f;

    TArray<ADifficultyZone*> OverlappingDifficultyZones; // To store overlapping zones
    
    UFUNCTION(Client, Reliable)
    void Client_DisableAllInput(APlayerController* PlayerController);

    void SpawnActorsForDifficultyWrapper();

public:
    FORCEINLINE USkeletalMeshComponent* GetEquippedWeapon() { return WeaponMesh; }
    FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() { return FirstPersonHand; }
    FORCEINLINE UCameraComponent* GetFPSCamera() { return FPSCamera; }

    FORCEINLINE void SetMaxPitchAngle(float _MaxPitchAngle) { MaxPitchAngle = _MaxPitchAngle;}
    FORCEINLINE void SetMinPitchAngle(float _MinPitchAngle) { MinPitchAngle = _MinPitchAngle;}
};
