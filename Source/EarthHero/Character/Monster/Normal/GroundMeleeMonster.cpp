// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundMeleeMonster.h"

#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Stat/DamageType/NormalDamageType.h"
#include "Kismet/KismetSystemLibrary.h"

AGroundMeleeMonster::AGroundMeleeMonster() : Super()
{
	AttackRange = 200.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Assets/ParagonMinions/Characters/Minions/Down_Minions/Animations/Melee/Attack_A_Montage.Attack_A_Montage'"));
	if (MontageFinder.Succeeded())
	{
		MyAnimMontage = MontageFinder.Object;
	}

	if(GetNetMode() == NM_Client) PrimaryActorTick.bCanEverTick = false;
}

void AGroundMeleeMonster::Attack()
{
	Multicast_Attack();
}

void AGroundMeleeMonster::Multicast_Attack_Implementation()
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if(SkeletalMesh)
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MyAnimMontage);
		}
	}
}

void AGroundMeleeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bCanAttack)
	{
		USkeletalMeshComponent* SkeletalMesh = GetMesh();
		const FVector Start = SkeletalMesh->GetSocketLocation(FName("weapon_sword_r"));
		const FVector End = SkeletalMesh->GetSocketLocation(FName("FX_Trail_01_R"));
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3)); //Monster_MeleeAttack

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		
		TArray<FHitResult> HitResults;
		
		bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			Start,
			End,
			5.f,
			ObjectTypes,
			false,
			IgnoredActors,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			3.f
		);
		
		if (bHit)
		{
			for (const FHitResult& Hit : HitResults)
			{
				AActor* Actor = Hit.GetActor();
				if (Actor)
				{
					AEHCharacter* EHCharacter = Cast<AEHCharacter>(Actor);
					if(EHCharacter)
					{
						UStatComponent* StatComponent = EHCharacter->StatComponent;
						if(StatComponent)
						{
							StatComponent->DamageTaken(30.f, UNormalDamageType::StaticClass(), Hit, GetController(), nullptr);
						}
					}
				}
			}
		}
	}
	
}