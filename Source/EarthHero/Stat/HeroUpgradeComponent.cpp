

#include "HeroUpgradeComponent.h"

UHeroUpgradeComponent::UHeroUpgradeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHeroUpgradeComponent::PushRandomHeroUpgrade()
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
	
	// 선택된 랜덤 업그레이드를 로그로 출력, UI쪽이 만들어지면 그쪽으로 보낼 예정
	for (const FHeroUpgradeStructure& Upgrade : RandomUpgrades)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Upgrade: %s"), *Upgrade.UpgradeName.ToString());
	}
}

// Called when the game starts
void UHeroUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

