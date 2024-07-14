

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
	OnRep_HeroUpgrades(); //HeroUpgrades 가 이시점에선 없다... 확인해야함
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

void UHeroUpgradeComponent::PushRandomHeroUpgrade_Implementation()
{
	RandomUpgrades.Empty();
	for (int i = 0; i < 3; i++)
	{
		RandomUpgradesIndex[i] = 0;
	}
	
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
	// 선택된 랜덤 업그레이드를 로그로 출력, UI쪽이 만들어지면 그쪽으로 보낼 예정
	for (int32 i = 0; i < ServerHeroUpgrades.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Upgrade: %s"), *ServerHeroUpgrades[i].UpgradeName.ToString());
		InGameHUD->SetIngameHUDHeroUpgrade(
			i,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			ServerHeroUpgrades[i].UpgradeName,
			ServerHeroUpgrades[i].Explanation[ServerHeroUpgrades[i].UpgradeLevel]
			);
	}
}

void UHeroUpgradeComponent::OnRep_HeroUpgrades()
{
	UE_LOG(LogClass, Warning, TEXT("OnRep_HeroUpgrades"));
	
	if (GetNetMode() == NM_Client && TabHUD && !HeroUpgrades.IsEmpty() &&HeroUpgrades.Num() >= 12)
	{
		for (int32 i = 0; i < HeroUpgrades.Num(); i++)
		{
			TabHUD->SetHeroUpgradeWidgetValues(
				i,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				HeroUpgrades[i].UpgradeName,
				HeroUpgrades[i].Explanation[HeroUpgrades[i].UpgradeLevel]);
		}
	}
}

void UHeroUpgradeComponent::ApplyHeroUpgrade_Implementation(int index)
{
	FHeroUpgradeStructure &SelectHeroUpgrade = HeroUpgrades[RandomUpgradesIndex[index]];
	UE_LOG(LogClass, Error, TEXT("Apply HeroUpgrade %s"), *SelectHeroUpgrade.UpgradeName.ToString());

	//InGameHUD -> 업그레이드 선택 비활성화
	SetFalseHUReady();
	
	switch (SelectHeroUpgrade.HeroUpgradeType)
	{
		case Pb_NormalAttackDamage :
				UHeroUpgradeLibrary::Pb_NormalAttackDamage(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_NormalAttackSpeed :
				UHeroUpgradeLibrary::Pb_NormalAttackSpeed(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_AttackSkillDamage :
				UHeroUpgradeLibrary::Pb_AttackSkillDamage(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_AttackSkillCooldown :
				UHeroUpgradeLibrary::Pb_AttackSkillCooldown(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_MovementSkillCooldown :
				UHeroUpgradeLibrary::Pb_MovementSkillCooldown(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_MaxHealth :
				UHeroUpgradeLibrary::Pb_MaxHealth(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_HealthRegenPerSecond :
				UHeroUpgradeLibrary::Pb_HealthRegenPerSecond(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Pb_MovementSpeed :
				UHeroUpgradeLibrary::Pb_MovementSpeed(SelectHeroUpgrade, GetBaseHeroStat(), GetHeroStat());
				break;
		case Wr_Berserker :
			break;
		case Wr_Guardian :
			break;
		case Wr_Leap :
			break;
		case Wr_Whirlwind :
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
			break;
		case St_IncreasedAmmoCapacity :
			break;
		case St_GrenadeEnhancement :
			break;
		case St_RocketBooster :
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
}

void UHeroUpgradeComponent::SetFalseHUReady_Implementation()
{
	InGameHUD->SetFalseHeroUpgradeReady();
}

void UHeroUpgradeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHeroUpgradeComponent, HeroUpgrades);
	// DOREPLIFETIME(UHeroUpgradeComponent, RandomUpgrades);
}


