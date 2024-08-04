
#include "ArcherCombatComponent.h"

UArcherCombatComponent::UArcherCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UArcherCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UArcherCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

