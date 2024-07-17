#include "ForceField.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EarthHero/Character/EHCharacter.h"

float MapEdgeLength = 201600.0f;
float MapDiagLength = MapEdgeLength * 1.1414;

AForceField::AForceField()
{
    PrimaryActorTick.bCanEverTick = true;

    ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldMesh"));
    RootComponent = ForceFieldMesh;
    
    ExpansionDuration = 20.0f; // Adjust as needed
    InitialScale = FVector(0.1f, 0.1f, 100.f); // Starting small

    static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprints/ForceField/FC_ExpansionCurve.FC_ExpansionCurve"));
    if (Curve.Succeeded())
    {
        ExpansionCurve = Curve.Object;
    }
}

void AForceField::BeginPlay()
{
    Super::BeginPlay();

    CurrentScale = InitialScale;
    ForceFieldMesh->SetWorldScale3D(CurrentScale);

    OnActorBeginOverlap.AddDynamic(this, &AForceField::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AForceField::OnOverlapEnd);

    SetupTimeline();
}

void AForceField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ForceFieldTimeline.TickTimeline(DeltaTime);
}

void AForceField::SetupTimeline()
{
    if (ExpansionCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("ExpandForceField"));
        ForceFieldTimeline.AddInterpFloat(ExpansionCurve, TimelineCallback);
        ForceFieldTimeline.SetLooping(false);
        ForceFieldTimeline.SetTimelineLength(ExpansionDuration);
        ForceFieldTimeline.PlayFromStart();
    }
}

void AForceField::ExpandForceField(float Value)
{
    float MaxScaleX = MapDiagLength / 100.0f; // Adjust based on initial scale
    float MaxScaleY = MapDiagLength / 100.0f; // Adjust based on initial scale

    FVector NewScale = InitialScale + FVector(MaxScaleX * Value, MaxScaleY * Value, 100.0f);
    CurrentScale = NewScale;
    ForceFieldMesh->SetWorldScale3D(CurrentScale);
}

void AForceField::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->SetIsInForceField(true);
            UE_LOG(LogTemp, Warning, TEXT("In force field: true"));
        }
    }
}

void AForceField::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
        {
            Character->SetIsInForceField(false);
            UE_LOG(LogTemp, Warning, TEXT("In force field: false"));
        }
    }
}