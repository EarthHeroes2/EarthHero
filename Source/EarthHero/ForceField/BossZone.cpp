#include "BossZone.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "EarthHero/Character/EHCharacter.h"
#include "Engine/Engine.h"

// Sets default values
ABP_BossZone::ABP_BossZone()
{
    PrimaryActorTick.bCanEverTick = true;

    BossZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossZoneMesh"));
    RootComponent = BossZoneMesh;

    CylinderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderComponent"));
    CylinderComponent->SetupAttachment(RootComponent);
    CylinderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        CylinderComponent->SetStaticMesh(CylinderMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Blueprints/ForceField/M_BossZone.M_BossZone'"));
    if (Material.Succeeded())
    {
        CylinderComponent->SetMaterial(0, Material.Object);
    }

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetupAttachment(RootComponent);
    CapsuleComponent->SetCapsuleHalfHeight(100.0f);
    CapsuleComponent->SetCapsuleRadius(50.0f);

    // Set custom collision to overlap all channels
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABP_BossZone::OnOverlapBegin);
    CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ABP_BossZone::OnOverlapEnd);
}

void ABP_BossZone::BeginPlay()
{
    Super::BeginPlay();
}

void ABP_BossZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->SetIsInBossZone(true);
        }
    }
}

void ABP_BossZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->SetIsInBossZone(false);
        }
    }
}
