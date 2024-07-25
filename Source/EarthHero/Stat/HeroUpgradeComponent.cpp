

#include "HeroUpgradeComponent.h"

#include "HeroUpgradeLibrary.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "EarthHero/HUD/TabHUDWidget.h"
#include "Net/UnrealNetwork.h"

UHeroUpgradeComponent::UHeroUpgradeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHeroUpgradeComponent::SetInGameHUD(UInGameHUD* ControllerInGameHUD)
{
	InGameHUD = ControllerInGameHUD;
}

void UHeroUpgradeComponent::SetTabHUD(UTabHUDWidget* ControllerTabHUD)
{
	TabHUD = ControllerTabHUD;
	OnRep_HeroUpgrades(HeroUpgrades);
}

void UHeroUpgradeComponent::SetStatComponent(int PlayerClass, UWarriorStatComponent *Wr, UMechanicStatComponent *Mc,
	UShooterStatComponent *Sh, UArcherStatComponent *Ar)
{
	OwnerClass = PlayerClass;
	switch (OwnerClass)
	{
	case Warrior :
		WarriorStatComponent = Wr;
		break;
	case Mechanic :
		MechanicStatComponent = Mc;
		break;
	case Shooter :
		ShooterStatComponent = Sh;
		break;
	case Archer :
		ArcherStatComponent = Ar;
		break;
	default:
		UE_LOG(LogClass, Warning, TEXT("HeroUpgradeComponent.SetStatComponent : UnKnown PlayerClass"));		
	}
}


// Called when the game starts
void UHeroUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

FStatStructure& UHeroUpgradeComponent::GetHeroStat()
{
	switch (OwnerClass)
	{
		case Warrior :
			return WarriorStatComponent->GetHeroStat();
		case Mechanic :
			return MechanicStatComponent->GetHeroStat();
		case Shooter :
			return ShooterStatComponent->GetHeroStat();
		case Archer :
			return ArcherStatComponent->GetHeroStat();
		default:
			UE_LOG(LogClass, Warning, TEXT("UHeroUpgradeComponent.GetHeroStat: 컴포넌트 오류"))
			static FStatStructure DummyStat;
			return DummyStat; // 또는 적절한 기본값 반환
	}
}

FStatStructure& UHeroUpgradeComponent::GetBaseHeroStat()
{
	switch (OwnerClass)
	{
	case Warrior :
		return WarriorStatComponent->GetBaseHeroStat();
	case Mechanic :
		return MechanicStatComponent->GetBaseHeroStat();
	case Shooter :
		return ShooterStatComponent->GetBaseHeroStat();
	case Archer :
		return ArcherStatComponent->GetBaseHeroStat();
	default:
		UE_LOG(LogClass, Warning, TEXT("UHeroUpgradeComponent.GetBaseHeroStat: 컴포넌트 오류"))
		static FStatStructure DummyStat;
		return DummyStat; // 또는 적절한 기본값 반환
	}
}

void UHeroUpgradeComponent::Delay()
{
	PushRandomHeroUpgrade();
}

void UHeroUpgradeComponent::PushRandomHeroUpgrade_Implementation()
{
	if (!IsSelectDone)
	{
		GetWorld()->GetTimerManager().SetTimer(SelectDoneTimerHandle, this, &UHeroUpgradeComponent::Delay, 0.5, false);
		return;
	}
	IsSelectDone = false;
	
	RandomUpgrades.Empty();
	for (int i = 0; i < 3; i++)
	{
		RandomUpgradesIndex[i] = 0;
	}

	// for (int i = 0; i < 12; i++)
	// {
	// 	if (HeroUpgrades[i].UpgradeLevel == 3) continue;
	// 	else if (HeroUpgrades[i].UpgradeLevel == -1) continue;
	// 	else AvailableUpgrades.Add(HeroUpgrades[i]);
	// }
	
	if (HeroUpgrades.Num() < 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough upgrades to choose from!"));
		return;
	}
	
	// 배열에서 랜덤하게 3개의 업그레이드를 선택합니다.
	while (RandomUpgrades.Num() < 3)
	{
		int32 RandomIndex = FMath::RandRange(0, HeroUpgrades.Num() - 1);
		FHeroUpgradeStructure SelectedUpgrade = HeroUpgrades[RandomIndex];
		if (SelectedUpgrade.UpgradeLevel == 3 || SelectedUpgrade.UpgradeLevel == -1)
			continue;
		if (!RandomUpgrades.Contains(SelectedUpgrade))
		{
			RandomUpgrades.Add(SelectedUpgrade);
			RandomUpgradesIndex[RandomUpgrades.Num() - 1] = RandomIndex;
		}
	}

	PushHeroUpgrade(RandomUpgrades);
}

//HeroUpgrades가 리플리케이트가 되긴 하지만, 더 빠른 선택을 위해 클라이언트 함수 호출
void UHeroUpgradeComponent::PushHeroUpgrade_Implementation(const TArray<FHeroUpgradeStructure> &ServerHeroUpgrades)
{
	for (int32 i = 0; i < ServerHeroUpgrades.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Upgrade: %s"), *ServerHeroUpgrades[i].UpgradeName.ToString());
		InGameHUD->SetIngameHUDHeroUpgrade(
			i,
			nullptr,
			ServerHeroUpgrades[i].UpgradeLevel,
			ServerHeroUpgrades[i].UpgradeName,
			ServerHeroUpgrades[i].Explanation[ServerHeroUpgrades[i].UpgradeLevel + 1]
			);
	}
}

void UHeroUpgradeComponent::OnRep_HeroUpgrades(const TArray<FHeroUpgradeStructure> &ServerHeroUpgrades)
{
	UE_LOG(LogClass, Warning, TEXT("OnRep_HeroUpgrades"));
	if (GetNetMode() == NM_Client && TabHUD && !ServerHeroUpgrades.IsEmpty())
	{
		for (int32 i = 0; i < ServerHeroUpgrades.Num(); i++)
		{
			//UE_LOG(LogClass, Warning, TEXT("HeroUpgrade : %s, Level : %d"), *ServerHeroUpgrades[i].UpgradeName.ToString(), ServerHeroUpgrades[i].UpgradeLevel)
			TabHUD->SetHeroUpgradeWidgetValues(
				i,
				nullptr,
				ServerHeroUpgrades[i].UpgradeLevel,
				ServerHeroUpgrades[i].UpgradeName,
				ServerHeroUpgrades[i].Explanation[ServerHeroUpgrades[i].UpgradeLevel]);
		}
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("OnRep_HeroUpgrades : Update Tab failed"));
	}
}

void UHeroUpgradeComponent::ApplyHeroUpgrade_Implementation(int index)
{
	FHeroUpgradeStructure &SelectHeroUpgrade = HeroUpgrades[RandomUpgradesIndex[index]];

	//InGameHUD -> 업그레이드 선택 비활성화
	SetFalseHUReady();
	
	switch (SelectHeroUpgrade.HeroUpgradeType)
	{
		case Pb_NormalAttackDamage :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::Pb_NormalAttackDamage(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_NormalAttackSpeed :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::Pb_NormalAttackSpeed(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_AttackSkillDamage :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::Pb_AttackSkillDamage(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_AttackSkillCooldown :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::Pb_AttackSkillCooldown(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_MovementSkillCooldown :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::Pb_MovementSkillCooldown(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_MaxHealth :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::Pb_MaxHealth(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_HealthRegenPerSecond :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
	
			UHeroUpgradeLibrary::Pb_HealthRegenPerSecond(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Pb_MovementSpeed :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
	
			UHeroUpgradeLibrary::Pb_MovementSpeed(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
			break;
		case Wr_Berserker :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
		
			UHeroUpgradeLibrary::WR_Berserker(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), WarriorStatComponent);
			RemoveUpgrade(Wr_Guardian);
			break;
		case Wr_Guardian :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
		
			UHeroUpgradeLibrary::WR_Guardian(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), WarriorStatComponent);
			RemoveUpgrade(Wr_Berserker);
			break;
		case Wr_Leap :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::WR_JumpEnhanced(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), WarriorStatComponent);
			break;
		case Wr_Whirlwind :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

			UHeroUpgradeLibrary::WR_WheelWind(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), WarriorStatComponent);
			break;
		case Mc_IncreasedDroneCount :
			break;
		case Mc_VitalityDrone :
			break;
		case Mc_ElectricShock :
			break;
		case Mc_Teleport :
			break;
		case St_Headshot :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
			
			UHeroUpgradeLibrary::St_Headshot(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), ShooterStatComponent);
			break;
		case St_IncreasedAmmoCapacity :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
			
			UHeroUpgradeLibrary::St_IncreasedAmmoCapacity(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), ShooterStatComponent);
			break;
		case St_GrenadeEnhancement :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
			
			UHeroUpgradeLibrary::St_GrenadeEnhancement(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), ShooterStatComponent);
			break;
		case St_RocketBooster :
			UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());
			
			UHeroUpgradeLibrary::St_RocketBooster(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat(), ShooterStatComponent);
			break;
		case Ac_PoisonedArrows :
			break;
		case Ac_CursedArrows :
			break;
		case Ac_FanArrowsEnhancement :
			break;
		case Ac_RollEnhancement :
			break;
		default :
			UE_LOG(LogClass, Error, TEXT("HeroUpgradeComponent.ApplyHeroUpgrade : Not Valid Hero Upgrade"));
	}

	//서버에서 수동으로 해줘야 할 업데이트
	switch (OwnerClass)
	{
		case Warrior :
			WarriorStatComponent->OnRep_HeroStat();
			break;
		case Mechanic :
			MechanicStatComponent->OnRep_HeroStat();
		break;
		case Shooter :
			ShooterStatComponent->OnRep_HeroStat();
		break;
		case Archer :
			ArcherStatComponent->OnRep_HeroStat();
		break;
	}
	Update_TabHeroUpgrades(HeroUpgrades);
	IsSelectDone = true;
}

void UHeroUpgradeComponent::RemoveUpgrade(int32 index)
{
	FHeroUpgradeStructure *FoundItem;
	int32 Index;
	
	FoundItem = Algo::FindByPredicate(HeroUpgrades, [](FHeroUpgradeStructure Upgrade)
	{
		return Upgrade.HeroUpgradeType == 8;
	});
	if (FoundItem)
	{
		Index = HeroUpgrades.IndexOfByKey(*FoundItem);
		HeroUpgrades[Index].UpgradeLevel == -1;
	}
}

void UHeroUpgradeComponent::SetFalseHUReady_Implementation()
{
	InGameHUD->SetFalseHeroUpgradeReady();
}

void UHeroUpgradeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UHeroUpgradeComponent::Update_TabHeroUpgrades_Implementation(const TArray<FHeroUpgradeStructure> &ServerHeroUpgrades)
{
	OnRep_HeroUpgrades(ServerHeroUpgrades);
}


