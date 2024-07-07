// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerState.h"

#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Stat/Structure/HeroUpgrageStructure.h"

AEHPlayerState::AEHPlayerState()
{
	WarriorStatComponent = CreateDefaultSubobject<UWarriorStatComponent>(TEXT("WarriorStatComponent"));
	MechanicStatComponent = CreateDefaultSubobject<UMechanicStatComponent>(TEXT("MechanicStatComponent"));
	ShooterStatComponent = CreateDefaultSubobject<UShooterStatComponent>(TEXT("ShooterStatComponent"));
	ArcherStatComponent = CreateDefaultSubobject<UArcherStatComponent>(TEXT("ArcherStatComponent"));
	

	static ConstructorHelpers::FObjectFinder<UDataTable> WarriorDataTable(TEXT("/Game/Data/HeroUpgrade/DT_WarriorHeroUpgrade.DT_WarriorHeroUpgrade"));
	if (WarriorDataTable.Succeeded())
	{
		WarriorHeroUpgradeDataTable = WarriorDataTable.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> MechanicDataTable(TEXT("/Game/Data/HeroUpgrade/DT_MechanicHeroUpgrade.DT_MechanicHeroUpgrade"));
	if (WarriorDataTable.Succeeded())
	{
		MechanicHeroUpgradeDataTable = WarriorDataTable.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> ShooterDataTable(TEXT("/Game/Data/HeroUpgrade/DT_ShooterHeroUpgrade.DT_ShooterHeroUpgrade"));
	if (WarriorDataTable.Succeeded())
	{
		ShooterHeroUpgradeDataTable = WarriorDataTable.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> ArcherDataTable(TEXT("/Game/Data/HeroUpgrade/DT_ArcherHeroUpgrade.DT_ArcherHeroUpgrade"));
	if (WarriorDataTable.Succeeded())
	{
		ArcherHeroUpgradeDataTable = WarriorDataTable.Object;
	}
	
	//히어로 업그레이드 컴포넌트
	HeroUpgradeComponent = CreateDefaultSubobject<UHeroUpgradeComponent>(TEXT("HeroUpgradeComponent"));

	//테스트를 위해 임시 카피 프로퍼티 호출
	PlayerClass = Shooter;
	CopyProperties(this);
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
			HeroUpgradeDataTable = WarriorHeroUpgradeDataTable;

			WarriorStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
		
		case Mechanic:
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ShooterStatComponent);
			DestroyComponent(ArcherStatComponent);
			HeroUpgradeDataTable = MechanicHeroUpgradeDataTable;

			MechanicStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
		
		case Shooter:
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ArcherStatComponent);
			HeroUpgradeDataTable = ShooterHeroUpgradeDataTable;

			ShooterStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
		
		case Archer:
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ShooterStatComponent);
			HeroUpgradeDataTable = ArcherHeroUpgradeDataTable;

			ArcherStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
	}
	
	LoadHeroUpgradeDatatable();
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

void AEHPlayerState::LoadHeroUpgradeDatatable()
{
	if (HeroUpgradeDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroUpgradeDatatable successfully loaded"));
		TArray<FHeroUpgradeStructure*> AllUpgrades;
		HeroUpgradeDataTable->GetAllRows(TEXT(""), AllUpgrades);

		HeroUpgradeComponent->HeroUpgrades.Empty();
		for (FHeroUpgradeStructure* Upgrade : AllUpgrades)
		{
			if (Upgrade)
			{
				HeroUpgradeComponent->HeroUpgrades.Add(*Upgrade);
			}
		}
		
		// 로그 출력
		for (const FHeroUpgradeStructure& Upgrade : HeroUpgradeComponent->HeroUpgrades)
		{
			UE_LOG(LogTemp, Warning, TEXT("Upgrade Name: %s, Explanation: %s, Level: %d"),
				*Upgrade.UpgradeName.ToString(),
				*Upgrade.Explanation.ToString(),
				Upgrade.UpgradeLevel);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroUpgradeDataTable is not valid"));
	}
}
