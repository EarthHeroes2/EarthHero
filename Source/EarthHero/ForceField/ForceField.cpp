#include "ForceField.h"
#include "Components/StaticMeshComponent.h"
#include "EarthHero/Character/EHCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

float MapEdgeLength = 201600.0f;
float MapDiagLength = MapEdgeLength * 1.1414;

AForceField::AForceField()
{
    PrimaryActorTick.bCanEverTick = true;

    ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForceFieldMesh"));
    RootComponent = ForceFieldMesh;
    
    ExpansionDuration = 1.f;
    InitialScale = FVector(0.1f, 0.1f, 300.f);

    static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Blueprints/ForceField/FC_ExpansionCurve.FC_ExpansionCurve"));
    if (Curve.Succeeded())
    {
        ExpansionCurve = Curve.Object;
    }

    bReplicates = true; // Enable replication for this actor
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

    FVector NewScale = InitialScale + FVector(MaxScaleX * Value, MaxScaleY * Value, 300.0f);
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

void AForceField::SetExpansionDuration(float NewDuration)
{
    if (NewDuration != ExpansionDuration)
    {
        ExpansionDuration = NewDuration;
        RestartTimeline();
        UE_LOG(LogTemp, Warning, TEXT("ExpansionDuration set to: %f"), ExpansionDuration);
    }
}

void AForceField::RestartTimeline()
{
    if (ForceFieldTimeline.IsPlaying())
    {
        ForceFieldTimeline.Stop();
    }
    SetupTimeline();
}

void AForceField::SetCustomCurve(UCurveFloat* NewCurve)
{
    if (NewCurve)
    {
        ExpansionCurve = NewCurve;
        RestartTimeline();
        UE_LOG(LogTemp, Warning, TEXT("CustomCurve set for ForceField"));
    }
}

void AForceField::OnRep_ExpansionCurve()
{
    // Restart timeline when curve is updated
    RestartTimeline();
}

void AForceField::OnRep_ExpansionDuration()
{
    // Restart timeline when duration is updated
    RestartTimeline();
}

void AForceField::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AForceField, ExpansionCurve);
    DOREPLIFETIME(AForceField, ExpansionDuration);
}
