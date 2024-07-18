#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ExpansionCurve, Category = "ForceField")
	UCurveFloat* ExpansionCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ExpansionDuration, Category = "ForceField")
	float ExpansionDuration;

	FTimeline ForceFieldTimeline;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "ForceField")
	void SetExpansionDuration(float NewDuration);

	UFUNCTION(BlueprintCallable, Category = "ForceField")
	void SetCustomCurve(UCurveFloat* NewCurve);

	UFUNCTION()
	void OnRep_ExpansionCurve();

	UFUNCTION()
	void OnRep_ExpansionDuration();

private:
	FVector InitialScale;
	FVector CurrentScale;

	UFUNCTION()
	void ExpandForceField(float Value);

	void SetupTimeline();
	void RestartTimeline();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
