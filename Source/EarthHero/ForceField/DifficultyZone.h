#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DifficultyZone.generated.h"

UCLASS()
class EARTHHERO_API ADifficultyZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADifficultyZone();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float Difficulty;

};
