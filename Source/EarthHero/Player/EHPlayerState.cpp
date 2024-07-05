// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerState.h"

#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/StatComponent.h"

AEHPlayerState::AEHPlayerState()
{
	WarriorStatComponent = CreateDefaultSubobject<UWarriorStatComponent>(TEXT("WarriorStatComponent"));
	MechanicStatComponent = CreateDefaultSubobject<UMechanicStatComponent>(TEXT("MechanicStatComponent"));
	ShooterStatComponent = CreateDefaultSubobject<UShooterStatComponent>(TEXT("ShooterStatComponent"));
	ArcherStatComponent = CreateDefaultSubobject<UArcherStatComponent>(TEXT("ArcherStatComponent"));
}

//현재 플레이어 스테이트에서 새로 생기는 플레이어 스테이트로 정보 복사
void AEHPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	
	AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (EHPlayerState)
	{
		EHPlayerState->PlayerClass = PlayerClass;
	}

	//직업을 확인하고 나머지 StatComponent를 비활성화
	switch (PlayerClass)
	{
		case Warrior:
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(ShooterStatComponent);
			DestroyComponent(ArcherStatComponent);
		case Mechanic:
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ShooterStatComponent);
			DestroyComponent(ArcherStatComponent);
		case Shooter:
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ArcherStatComponent);
		case Archer:
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ShooterStatComponent);
	}
		
}

UStatComponent* AEHPlayerState::GetStatComponent()
{
	switch (PlayerClass)
	{
	case Warrior:
		return WarriorStatComponent;
	case Mechanic:
		return MechanicStatComponent;
	case Shooter:
		return ShooterStatComponent;
	case Archer:
		return ArcherStatComponent;
	}
	return nullptr;
}

void AEHPlayerState::DestroyComponent(UStatComponent* Target)
{
	if (Target)
	{
		Target->UnregisterComponent();
		Target->DestroyComponent();
	}
	Target = nullptr;
}
