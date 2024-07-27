#include "DifficultyZone.h"
#include "EarthHero/Character/EHCharacter.h"
#include "Components/BoxComponent.h"

ADifficultyZone::ADifficultyZone()
{
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &ADifficultyZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ADifficultyZone::OnOverlapEnd);

	Difficulty = 1.0f;
}

void ADifficultyZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
		{
			if (Character->IsLocallyControlled())
			{
				Character->AddDifficultyZone(this);
			}
		}
	}
}

void ADifficultyZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		if (AEHCharacter* Character = Cast<AEHCharacter>(OtherActor))
		{
			if (Character->IsLocallyControlled())
			{
				Character->RemoveDifficultyZone(this);
			}
		}
	}
}

void ADifficultyZone::BeginPlay()
{
	Super::BeginPlay();
}

void ADifficultyZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
