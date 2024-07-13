

#include "HeroUpgradeComponent.h"

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


// Called when the game starts
void UHeroUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UHeroUpgradeComponent::PushRandomHeroUpgrade_Implementation()
{
	RandomUpgrades.Empty();
	
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
	// if (HeroUpgrades.IsEmpty())
	// {
	// 	UE_LOG(LogClass, Warning, TEXT("HeroUpgrades is Empty"));
	// }
	
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


void UHeroUpgradeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHeroUpgradeComponent, HeroUpgrades);
	// DOREPLIFETIME(UHeroUpgradeComponent, RandomUpgrades);
}


