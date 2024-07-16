#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "ForceField.generated.h"

UCLASS()
class EARTHHERO_API AForceField : public AActor
{
	GENERATED_BODY()
    
public:    
	AForceField();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ForceFieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForceField")
	UCurveFloat* ExpansionCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForceField")
	float ExpansionDuration;

	FTimeline ForceFieldTimeline;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

private:
	FVector InitialScale;
	FVector CurrentScale;

	UFUNCTION()
	void ExpandForceField(float Value);

	void SetupTimeline();
};
