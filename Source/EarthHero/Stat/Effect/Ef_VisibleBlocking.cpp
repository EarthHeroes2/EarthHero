// Fill out your copyright notice in the Description page of Project Settings.


#include "Ef_VisibleBlocking.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Enum/Enums.h"

AEf_VisibleBlocking::AEf_VisibleBlocking()
{
	ExponentialHeightFogComponent = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFogComponent"));
	RootComponent = ExponentialHeightFogComponent;
	ExponentialHeightFogComponent->SetIsReplicated(true);
}

void AEf_VisibleBlocking::ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable, bool InbIsPermanent, bool InbShouldRefreshDuration)
{
	UE_LOG(LogClass, Error, TEXT("SpawmEffects2.5"));
	
	// if (GetNetMode() != NM_ListenServer)
	// {
	// 	UE_LOG(LogClass, Error, TEXT("SpawmEffects3"));
	// 	SetFogSetting();
	// 	AttachToActor(InTargetActor, FAttachmentTransformRules::KeepRelativeTransform);
	// 	GetWorld()->GetTimerManager().SetTimer(VisibleBlockingHandle, this, &AEf_VisibleBlocking::ResetVisibleBlockingClient, InDuration, false);
	// }
	// else
	// {
	// 	UE_LOG(LogClass, Error, TEXT("SpawmEffects4"));
	// 	EffectType = Ef_VisibleBlocking;
	// 	Super::ApplyEffect(InTargetActor, InEffectValue, InDuration, InbIsStackable, InbIsPermanent, InbShouldRefreshDuration);
	// 	if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
	// 	{
	// 		UE_LOG(LogClass, Error, TEXT("SpawmEffects5"));
	// 		AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
	// 	}
	// }
	if (!IsRunningDedicatedServer())
	{
		UE_LOG(LogClass, Error, TEXT("SpawmEffects3"));
		SetFogSetting();
		AttachToActor(InTargetActor, FAttachmentTransformRules::KeepRelativeTransform);
		GetWorld()->GetTimerManager().SetTimer(VisibleBlockingHandle, this, &AEf_VisibleBlocking::ResetVisibleBlockingClient, InDuration, false);
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("SpawmEffects4"));
		EffectType = Ef_VisibleBlocking;
		Super::ApplyEffect(InTargetActor, InEffectValue, InDuration, InbIsStackable, InbIsPermanent, InbShouldRefreshDuration);
		if (AEHCharacter* Character = Cast<AEHCharacter>(TargetActor))
		{
			UE_LOG(LogClass, Error, TEXT("SpawmEffects5"));
			AttachToActor(TargetActor, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AEf_VisibleBlocking::UpgradeEffect(float InEffectValue)
{
	Super::UpgradeEffect(InEffectValue);

	if (bRefresh && !bIsStackable)
	{
		bRefresh = false;
	}
}

void AEf_VisibleBlocking::ResetEffect()
{
	Super::ResetEffect();
}

void AEf_VisibleBlocking::SetFogSetting()
{
	ExponentialHeightFogComponent->FogDensity = 2.0f;
	ExponentialHeightFogComponent->SkyAtmosphereAmbientContributionColorScale = FLinearColor(0, 0, 0, 0);
	ExponentialHeightFogComponent->DirectionalInscatteringExponent = 1000.f;
	ExponentialHeightFogComponent->bEnableVolumetricFog = true;
	ExponentialHeightFogComponent->VolumetricFogAlbedo = FColor(0, 0, 0, 0);
}

void AEf_VisibleBlocking::ResetVisibleBlockingClient()
{
	Destroy();
}
