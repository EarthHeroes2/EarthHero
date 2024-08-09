// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"

#include "Components/WidgetComponent.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
	// if(GetNetMode() != NM_ListenServer) PrimaryActorTick.bCanEverTick = false;
	// else PrimaryActorTick.bCanEverTick = true
	PrimaryActorTick.bCanEverTick = true;
	
	MonsterStatHUDComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterStatHUD"));
	MonsterStatHUDComponent->SetupAttachment(RootComponent);
	
	MonsterStatComponent = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStatComponent"));
	MonsterStatComponent->SetIsReplicated(true);

	//이 값은 자식에서 변경해줘야함. 기본값을 위해 존재할 뿐임
	BossNumber = NotBoss;
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	MonsterStatHUDComponent->SetVisibility(false, true);
	MonsterStatComponent->Monster = this;
	MonsterStatComponent->InitializeStatData();
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonsterBase::AddEffect(UTexture2D* EffectImage, int ServerEffectType, float Duration)
{
	if (MonsterStatComponent)
	{
		MonsterStatComponent->UpdateEffectImage(EffectImage, ServerEffectType, Duration);
	}
}

void AMonsterBase::SetMaxFlySpeed_Implementation(float NewFlySpeed)
{
	GetCharacterMovement()->MaxFlySpeed = NewFlySpeed;
}

void AMonsterBase::SetMaxWalkSpeed_Implementation(float NewWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
}

void AMonsterBase::SetVelocity_Implementation(FVector Velocity)
{
	GetCharacterMovement()->Velocity = Velocity;
}

void AMonsterBase::SetIsCanMove_Implementation(bool SetCanMove)
{
	IsCanMove = SetCanMove;
	AAIControllerBase *AIController = Cast<AAIControllerBase>(GetController());
	if (AIController)
	{
		AIController->SetIsCanMove(SetCanMove);
	}
}

//자식에서 구현 (근거리용)
void AMonsterBase::Attack()
{
}
//자식에서 구현 (원거리용)
//SpawnNormalVector는 타겟 플레이어를 향하는 단위벡터
void AMonsterBase::Attack(FVector SpawnNormalVector)
{
}

//스킬들 (자식에서 필요하다면 구현)
void AMonsterBase::Skill1()
{
}
void AMonsterBase::Skill2()
{
}
void AMonsterBase::Skill3()
{
}
void AMonsterBase::Skill4()
{
}

void AMonsterBase::Skill1(FVector SpawnNormalVector)
{
}
void AMonsterBase::Skill2(FVector SpawnNormalVector)
{
}
void AMonsterBase::Skill3(FVector SpawnNormalVector)
{
}
void AMonsterBase::Skill4(FVector SpawnNormalVector)
{
}

void AMonsterBase::Skill1(AEHCharacter* TargetCharacter)
{
}
void AMonsterBase::Skill2(AEHCharacter* TargetCharacter)
{
}
void AMonsterBase::Skill3(AEHCharacter* TargetCharacter)
{
}
void AMonsterBase::Skill4(AEHCharacter* TargetCharacter)
{
}



void AMonsterBase::Multicast_Attack_Implementation()
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if(SkeletalMesh)
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackAnimMontage);
		}
	}
}

void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
	if (PlayingGameMode)
	{
		if (PlayingGameMode->IsDebugMode)
		{
			if (GetNetMode() == NM_ListenServer)
			{
				DoMeleeTrace();
			}
		}
		else
		{
			if (IsRunningDedicatedServer())
			{
				DoMeleeTrace();
			}
		}
	}
}

void AMonsterBase::DoMeleeTrace()
{
	if(bMeleeAttackRange)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3)); //Monster_MeleeAttack

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		
		TArray<FHitResult> HitResults;
		
		bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
			GetWorld(),
			StartLocation,
			EndLocation,
			Radius,
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
			for (auto Hit : HitResults)
			{
				CheckAttackedEnemy(Hit);
			}
		}
	}
	else
	{
		AttackedEnemy.Empty();
	}
}

void AMonsterBase::CheckAttackedEnemy(FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor && !AttackedEnemy.Contains(HitActor))
	{
		AttackedEnemy.Add(HitActor);
		
		if (HitActor)
		{
			// 데미지 적용
			MonsterStatComponent->GiveNormalDamage(HitActor, InDamage);
		}
	}
}

