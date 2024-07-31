// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthHero/Stat/Effect/EffectBase.h"

#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "EarthHero/Stat/Structure/EffectStructure.h"

TMap<AActor*, TMap<TSubclassOf<AEffectBase>, AEffectBase*>> AEffectBase::EffectMap;
TArray<FEffectStructure*> AEffectBase::EffectArray;

// Sets default values
AEffectBase::AEffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEffectBase::ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable, bool InbIsPermanent, bool InbShouldRefreshDuration)
{
	TargetActor = InTargetActor;
	AppliedEffectValue = InEffectValue;
	EffectDuration = InDuration;
	bIsStackable = InbIsStackable;
	bIsPermanent = InbIsPermanent;
	bShouldRefreshDuration = InbShouldRefreshDuration;
	
	TMap<TSubclassOf<AEffectBase>, AEffectBase*>* TargetMap = EffectMap.Find(TargetActor);
	if (TargetMap)
	{
		// 이미 존재하는 효과이면..
		AEffectBase** ExistingEffectPtr = TargetMap->Find(GetClass());
		if (ExistingEffectPtr)
		{
			AEffectBase* ExistingEffect = *ExistingEffectPtr;

			//중첩 가능한 효과이면...
			if (bIsStackable)
			{
				//중첩 효과 강화하는 함수 실행해야 한다.
				ExistingEffect->AppliedEffectValue += InEffectValue;
				ExistingEffect->UpgradeEffect(InEffectValue);
			}
            
			// 영구효과가 아니고, 갱신 가능한 효과인 경우, 기존 타이머를 클리어하고 다시 설정하여 지속 시간을 갱신
			if (!ExistingEffect->bIsPermanent && bShouldRefreshDuration)
			{
				ExistingEffect->GetWorld()->GetTimerManager().ClearTimer(ExistingEffect->EffectTimerHandle);
				ExistingEffect->GetWorld()->GetTimerManager().SetTimer(ExistingEffect->EffectTimerHandle, ExistingEffect, &AEffectBase::ResetEffect, InDuration, false);
				if (AEHCharacter *Hero = Cast<AEHCharacter>(TargetActor))
				{
					//본인 HUD에 띄우고
					if (AEHPlayerController *PlayerController = Cast<AEHPlayerController>(Hero->GetController()))
					{
						if (EffectArray.Num() < EffectType)
						{
							UE_LOG(LogClass, Error, TEXT("AEffectBase::EffectArray has not initialized"));
						}
						else
						{
							PlayerController->Client_AddEffect(EffectArray[EffectType]->EffectImage, EffectArray[EffectType]->EffectType, InDuration);
						}
					}
					if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
					{
						PlayingGameMode->UpdatePlayerStateImage();
					}
				}
				bRefresh = true;
			}
		}
		else
		{
			if (!bIsPermanent)
			{
				GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AEffectBase::ResetEffect, InDuration, false);
			}

			//효과 적용 목록에 추가
			EffectMap.Add(TargetActor, TMap<TSubclassOf<AEffectBase>, AEffectBase*>());
			TargetMap = EffectMap.Find(TargetActor);
			if (AEHCharacter *Hero = Cast<AEHCharacter>(TargetActor))
			{
				if (AEHPlayerController *PlayerController = Cast<AEHPlayerController>(Hero->GetController()))
				{
					if (EffectArray.Num() < EffectType)
					{
						UE_LOG(LogClass, Error, TEXT("AEffectBase::EffectArray has not initialized"));
					}
					else
					{
						PlayerController->Client_AddEffect(EffectArray[EffectType]->EffectImage, EffectArray[EffectType]->EffectType, InDuration);
					}
				}
				if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
				{
					PlayingGameMode->UpdatePlayerStateImage();
				}
			}
		}
		return;
	}
	else //첫 효과 적용
	{
		if (!bIsPermanent)
		{
			GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AEffectBase::ResetEffect, InDuration, false);
		}

		//효과 적용 목록에 추가
		EffectMap.Add(TargetActor, TMap<TSubclassOf<AEffectBase>, AEffectBase*>());
		TargetMap = EffectMap.Find(TargetActor);
		if (AEHCharacter *Hero = Cast<AEHCharacter>(TargetActor))
		{
			if (AEHPlayerController *PlayerController = Cast<AEHPlayerController>(Hero->GetController()))
			{
				if (EffectArray.Num() < EffectType)
				{
					UE_LOG(LogClass, Error, TEXT("AEffectBase::EffectArray has not initialized"));
				}
				else
				{
					PlayerController->Client_AddEffect(EffectArray[EffectType]->EffectImage, EffectArray[EffectType]->EffectType, InDuration);
				}
			}
			if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
			{
				PlayingGameMode->UpdatePlayerStateImage();
			}
		}
	}
	TargetMap->Add(GetClass(), this);
	
}

void AEffectBase::UpgradeEffect(float InEffectValue)
{
}

void AEffectBase::ResetEffect()
{
	// 효과 리셋 로직 구현
	// 서브클래스에서 오버라이드하여 구체적인 효과 리셋 로직을 구현합니다.

	//영구 효과는 지울 수 없다.
	if (!bIsPermanent)
	{
		TMap<TSubclassOf<AEffectBase>, AEffectBase*>* TargetMap = EffectMap.Find(TargetActor);
		if (TargetMap)
		{
			TargetMap->Remove(GetClass());
			if (TargetMap->Num() == 0)
			{
				EffectMap.Remove(TargetActor);
			}
		}
	}

	// 이 액터를 파괴하여 메모리 해제
	Destroy();
}


