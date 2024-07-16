// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerState.h"

#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Enum/Enums.h"
#include "EarthHero/PlayerState/GameOverPlayerState.h"
#include "EarthHero/Stat/Structure/HeroUpgrageStructure.h"
#include "Net/UnrealNetwork.h"

AEHPlayerState::AEHPlayerState()
{
	WarriorStatComponent = CreateDefaultSubobject<UWarriorStatComponent>(TEXT("WarriorStatComponent"));
	MechanicStatComponent = CreateDefaultSubobject<UMechanicStatComponent>(TEXT("MechanicStatComponent"));
	ShooterStatComponent = CreateDefaultSubobject<UShooterStatComponent>(TEXT("ShooterStatComponent"));
	ArcherStatComponent = CreateDefaultSubobject<UArcherStatComponent>(TEXT("ArcherStatComponent"));

	//히어로 업그레이드 컴포넌트
	HeroUpgradeComponent = CreateDefaultSubobject<UHeroUpgradeComponent>(TEXT("HeroUpgradeComponent"));

	WarriorStatComponent->SetIsReplicated(true);
	MechanicStatComponent->SetIsReplicated(true);
	ShooterStatComponent->SetIsReplicated(true);
	ArcherStatComponent->SetIsReplicated(true);
	HeroUpgradeComponent->SetIsReplicated(true);

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
	
	//리플리케이트 가능하게 설정, 액터가 리플리케이트 되면 컴포넌트도 리플리케이트 된다.
	bReplicates = true;
	bAlwaysRelevant = true; // 항상 복제되도록 설정 (선택 사항)
}

//인게임 -> 게임종료로 필요한 데이터 복사
void AEHPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	UE_LOG(LogTemp, Error, TEXT("Copy Properties - EH"));
	
	AGameOverPlayerState* GameOverPlayerState = Cast<AGameOverPlayerState>(PlayerState);
	
	if (GameOverPlayerState)
		GameOverPlayerState->TestVar = PlayerClass;

	GameOverPlayerState->TestFunc();

	//EHPlayerState->SetStatComponent(); //이것은 사정상 lobbyPlayerState로 이동되었습니다 - 박정익
}

void AEHPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	//패키징 시 주석 처리해야 함
	PlayerClass = Warrior; ////
	IsCopyPropertiesEnd = true; ////
	GetWorldTimerManager().SetTimer(SetStatComponentTimerHandle, this, &AEHPlayerState::SetStatComponent, 0.5f,true);  ////
}

void AEHPlayerState::SetStatComponent()
{
	if (IsCopyPropertiesEnd)
	{
		GetWorldTimerManager().ClearTimer(SetStatComponentTimerHandle); //
		
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

		HeroUpgradeComponent->SetStatComponent(PlayerClass, WarriorStatComponent, MechanicStatComponent, ShooterStatComponent, ArcherStatComponent);
		LoadHeroUpgradeDatatable();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EHPlayerState: failed CopyProperties"));
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

UHeroUpgradeComponent* AEHPlayerState::GetHeroUpgradeComponent()
{
	return HeroUpgradeComponent;
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
	// for (const FHeroUpgradeStructure& Upgrade : HeroUpgradeComponent->HeroUpgrades)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Upgrade Name: %s, Explanation: %s, Level: %d"),
	// 		*Upgrade.UpgradeName.ToString(),
	// 		*Upgrade.Explanation[0].ToString(),
	// 		Upgrade.UpgradeLevel);
	// }

	IsSetStatComponentEnd = true;
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
