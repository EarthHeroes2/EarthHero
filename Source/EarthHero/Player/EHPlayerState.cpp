// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerState.h"

#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Enum/Enums.h"
#include "EarthHero/Stat/Structure/HeroUpgrageStructure.h"
#include "Net/UnrealNetwork.h"

AEHPlayerState::AEHPlayerState()
{
	//isdedicatedServer로 바꿔야함
		WarriorStatComponent = CreateDefaultSubobject<UWarriorStatComponent>(TEXT("WarriorStatComponent"));
		MechanicStatComponent = CreateDefaultSubobject<UMechanicStatComponent>(TEXT("MechanicStatComponent"));
		ShooterStatComponent = CreateDefaultSubobject<UShooterStatComponent>(TEXT("ShooterStatComponent"));
		ArcherStatComponent = CreateDefaultSubobject<UArcherStatComponent>(TEXT("ArcherStatComponent"));
	

		WarriorStatComponent->SetIsReplicated(true);
		MechanicStatComponent->SetIsReplicated(true);
		ShooterStatComponent->SetIsReplicated(true);
		ArcherStatComponent->SetIsReplicated(true);

		static ConstructorHelpers::FObjectFinder<UDataTable> WarriorDataTable(TEXT("/Game/Data/HeroUpgrade/DT_WarriorHeroUpgrade.DT_WarriorHeroUpgrade"));
		if (WarriorDataTable.Succeeded())
		{
			WarriorHeroUpgradeDataTable = WarriorDataTable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UDataTable> MechanicDataTable(TEXT("/Game/Data/HeroUpgrade/DT_MechanicHeroUpgrade.DT_MechanicHeroUpgrade"));
		if (MechanicDataTable.Succeeded())
		{
			MechanicHeroUpgradeDataTable = MechanicDataTable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UDataTable> ShooterDataTable(TEXT("/Game/Data/HeroUpgrade/DT_ShooterHeroUpgrade.DT_ShooterHeroUpgrade"));
		if (ShooterDataTable.Succeeded())
		{
			ShooterHeroUpgradeDataTable = ShooterDataTable.Object;
		}
		static ConstructorHelpers::FObjectFinder<UDataTable> ArcherDataTable(TEXT("/Game/Data/HeroUpgrade/DT_ArcherHeroUpgrade.DT_ArcherHeroUpgrade"));
		if (ArcherDataTable.Succeeded())
		{
			ArcherHeroUpgradeDataTable = ArcherDataTable.Object;
		}
	
		//히어로 업그레이드 컴포넌트
		HeroUpgradeComponent = CreateDefaultSubobject<UHeroUpgradeComponent>(TEXT("HeroUpgradeComponent"));
	
	//리플리케이트 가능하게 설정, 액터가 리플리케이트 되면 컴포넌트도 리플리케이트 된다.
	bReplicates = true;
	bAlwaysRelevant = true; // 항상 복제되도록 설정 (선택 사항)
}

//현재 플레이어 스테이트에서 새로 생기는 플레이어 스테이트로 정보 복사
void AEHPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	UE_LOG(LogTemp, Error, TEXT("Copy Properties Activated"));
	
	AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(PlayerState);
	if (EHPlayerState)
	{
		EHPlayerState->PlayerClass = PlayerClass;
	}

	EHPlayerState->IsCopyPropertiesEnd = true;
}

void AEHPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
	{
		UE_LOG(LogTemp, Log, TEXT("This instance is dedicated."));
		PlayerClass = Shooter;
		IsCopyPropertiesEnd = true;
		GetWorldTimerManager().SetTimer(SetStatComponentTimerHandle, this, &AEHPlayerState::SetStatComponent, 0.5f,true);
	}
}

void AEHPlayerState::SetStatComponent()
{
	if (IsCopyPropertiesEnd)
	{
		GetWorldTimerManager().ClearTimer(SetStatComponentTimerHandle);
		
		//직업을 확인하고 나머지 StatComponent를 비활성화, 서버에서 파괴하면 클라이언트도 파괴
		switch (PlayerClass)
		{
		case Warrior:
			UE_LOG(LogTemp, Error, TEXT("EHPlayerState: Possesed Character is Warrior"));
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(ShooterStatComponent);
			DestroyComponent(ArcherStatComponent);
			HeroUpgradeDataTable = WarriorHeroUpgradeDataTable;

			WarriorStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
		
		case Mechanic:
			UE_LOG(LogTemp, Error, TEXT("EHPlayerState: Possesed Character is Mechanic"));
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ShooterStatComponent);
			DestroyComponent(ArcherStatComponent);
			HeroUpgradeDataTable = MechanicHeroUpgradeDataTable;

			MechanicStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
		
		case Shooter:
			UE_LOG(LogTemp, Error, TEXT("EHPlayerState: Possesed Character is Shooter"));
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ArcherStatComponent);
			HeroUpgradeDataTable = ShooterHeroUpgradeDataTable;

			ShooterStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
		
		case Archer:
			UE_LOG(LogTemp, Error, TEXT("EHPlayerState: Possesed Character is Archer"));
			DestroyComponent(MechanicStatComponent);
			DestroyComponent(WarriorStatComponent);
			DestroyComponent(ShooterStatComponent);
			HeroUpgradeDataTable = ArcherHeroUpgradeDataTable;

			ArcherStatComponent->SetHeroUpgradeStComp(HeroUpgradeComponent);
			break;
			
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid Player Class"));
			return;
		}
	
		LoadHeroUpgradeDatatable();
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("EHPlayerState: failed CopyProperties"));
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

void AEHPlayerState::LoadHeroUpgradeDatatable()
{
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
			*Upgrade.Explanation[0].ToString(),
			Upgrade.UpgradeLevel);
	}

	IsSetStatCompoentEnd = true;
}


void AEHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEHPlayerState, PlayerClass);
	DOREPLIFETIME(AEHPlayerState, IsCopyPropertiesEnd);
	DOREPLIFETIME(AEHPlayerState, ShooterStatComponent);
	DOREPLIFETIME(AEHPlayerState, ArcherStatComponent);
	DOREPLIFETIME(AEHPlayerState, MechanicStatComponent);
	DOREPLIFETIME(AEHPlayerState, WarriorStatComponent);
	DOREPLIFETIME(AEHPlayerState, HeroUpgradeComponent);
}
