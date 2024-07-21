#include "TabHUDWidget.h"
#include "TeamMemberWidget.h"
#include "BasicStatsWidget.h"
#include "StatusWidget.h"
#include "HeroUpgradeWidget.h"
#include "Components/EditableTextBox.h"

#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"

bool UTabHUDWidget::Initialize()
{
	Super::Initialize();
	
	TabUserInfoArray.Add(BP_TabUserInfo_1);
	TabUserInfoArray.Add(BP_TabUserInfo_2);
	TabUserInfoArray.Add(BP_TabUserInfo_3);
	TabUserInfoArray.Add(BP_TabUserInfo_4);
	TabUserBasicStatsArray.Add(BP_TabBasicStatsInfo_1);
	TabUserBasicStatsArray.Add(BP_TabBasicStatsInfo_2);
	TabUserBasicStatsArray.Add(BP_TabBasicStatsInfo_3);
	TabUserBasicStatsArray.Add(BP_TabBasicStatsInfo_4);

	InterpolatedPositions.Add(FVector2D(0, 0));
	InterpolatedPositions.Add(FVector2D(0, 0));
	InterpolatedPositions.Add(FVector2D(0, 0));
	InterpolatedPositions.Add(FVector2D(0, 0));
	InterpolatedRotations.Add(0);
	InterpolatedRotations.Add(0);
	InterpolatedRotations.Add(0);
	InterpolatedRotations.Add(0);
	
	
	return true;
}

void UTabHUDWidget::SetTeamMemberWidgetValues(int32 Index, FString Name, EClassType ClassType, FText PlayerNumber, int32 Level, float Exp, float HealthProgress)
{
	UTeamMemberWidget* TeamMemberWidget = nullptr;

	switch(Index)
	{
	case 1:
		TeamMemberWidget = BP_TabUserInfo_1;
		break;
	case 2:
		TeamMemberWidget = BP_TabUserInfo_2;
		break;
	case 3:
		TeamMemberWidget = BP_TabUserInfo_3;
		break;
	case 4:
		TeamMemberWidget = BP_TabUserInfo_4;
		break;
	default:
		return;
	}

	if (TeamMemberWidget)
	{
		TeamMemberWidget->SetName(Name);
		TeamMemberWidget->SetClass(ClassType);
		TeamMemberWidget->SetPlayerNumber(PlayerNumber);
		TeamMemberWidget->SetLevel(Level);
		TeamMemberWidget->SetExp(Exp);
		TeamMemberWidget->SetProgress(HealthProgress);
	}
}

void UTabHUDWidget::SetBasicStatsWidgetValues(int32 Index, FText KillScore, FText TotalDamage, FText TotalShield, FText TotalHeal)
{
	UBasicStatsWidget* BasicStatsWidget = nullptr;

	switch(Index)
	{
	case 1:
		BasicStatsWidget = BP_TabBasicStatsInfo_1;
		break;
	case 2:
		BasicStatsWidget = BP_TabBasicStatsInfo_2;
		break;
	case 3:
		BasicStatsWidget = BP_TabBasicStatsInfo_3;
		break;
	case 4:
		BasicStatsWidget = BP_TabBasicStatsInfo_4;
		break;
	default:
		return;
	}

	if (BasicStatsWidget)
	{
		BasicStatsWidget->SetKillScore(KillScore);
		BasicStatsWidget->SetTotalDamage(TotalDamage);
		BasicStatsWidget->SetTotalShield(TotalShield);
		BasicStatsWidget->SetTotalHeal(TotalHeal);
	}
}

// 계속 호출
void UTabHUDWidget::UpdateForceField(int32 ForceFieldIndex, float CurrentTime, float GrowthDuration)
{
	if (BP_WorldMap)
	{
		UE_LOG(LogTemp, Log, TEXT("CurrentTime: %f"), CurrentTime);
		BP_WorldMap->UpdateForceField(ForceFieldIndex, CurrentTime, GrowthDuration);
	}
}

// 한 번 호출
void UTabHUDWidget::SetForceFieldAlignment(int32 ForceFieldIndex, const FVector2D& Alignment)
{
	if (BP_WorldMap)
	{
		BP_WorldMap->SetForceFieldAlignment(ForceFieldIndex - 1, Alignment); // Convert to 0-based index
	}
}

void UTabHUDWidget::SetStatusWidgetValues(FText AttackDamage, FText AttackSpeed, FText AttackSkillDamage, FText AttackSkillCooltime, FText MoveSkillCooltime, FText CurrentHealth, FText CurrentShield, FText MaxHealth, FText HealthRegenerationRate, FText MoveSpeed)
{
	if (BP_TabStatusInfo)
	{
		BP_TabStatusInfo->SetAttackDamage(AttackDamage);
		BP_TabStatusInfo->SetAttackSpeed(AttackSpeed);
		BP_TabStatusInfo->SetAttackSkillDamage(AttackSkillDamage);
		BP_TabStatusInfo->SetAttackSkillCooltime(AttackSkillCooltime);
		BP_TabStatusInfo->SetMoveSkillCooltime(MoveSkillCooltime);
		BP_TabStatusInfo->SetCurrentHealth(CurrentHealth);
		BP_TabStatusInfo->SetCurrentShield(CurrentShield);
		BP_TabStatusInfo->SetMaxHealth(MaxHealth);
		BP_TabStatusInfo->SetHealthRegenerationRate(HealthRegenerationRate);
		BP_TabStatusInfo->SetMoveSpeed(MoveSpeed);
	}
}

void UTabHUDWidget::SetHeroUpgradeWidgetValues(int32 Index, UTexture2D* UpgradeImage, int Level, FText UpgradeName, FText UpgradeDesc)
{
	UHeroUpgradeWidget* HeroUpgradeWidget = nullptr;

	switch(Index)
	{
	case 0:
		HeroUpgradeWidget = BP_TabHeroUpgrade_0;
		break;
	case 1:
		HeroUpgradeWidget = BP_TabHeroUpgrade_1;
		break;
	case 2:
		HeroUpgradeWidget = BP_TabHeroUpgrade_2;
		break;
	case 3:
		HeroUpgradeWidget = BP_TabHeroUpgrade_3;
		break;
	case 4:
		HeroUpgradeWidget = BP_TabHeroUpgrade_4;
		break;
	case 5:
		HeroUpgradeWidget = BP_TabHeroUpgrade_5;
		break;
	case 6:
		HeroUpgradeWidget = BP_TabHeroUpgrade_6;
		break;
	case 7:
		HeroUpgradeWidget = BP_TabHeroUpgrade_7;
		break;
	case 8:
		HeroUpgradeWidget = BP_TabHeroUpgrade_8;
		break;
	case 9:
		HeroUpgradeWidget = BP_TabHeroUpgrade_9;
		break;
	case 10:
		HeroUpgradeWidget = BP_TabHeroUpgrade_10;
		break;
	case 11:
		HeroUpgradeWidget = BP_TabHeroUpgrade_11;
		break;
	default:
		return;
	}

	if (HeroUpgradeWidget)
	{
		HeroUpgradeWidget->SetUpgradeImage(UpgradeImage);
		HeroUpgradeWidget->SetUpgradeName(UpgradeName);
		HeroUpgradeWidget->SetUpgradeDesc(UpgradeDesc);

		UTexture2D* GoldStar = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/Full_Star_Yellow.Full_Star_Yellow'")));
		UTexture2D* WhiteStar = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/Empty_Star.Empty_Star'")));

		switch(Level)
		{
		case 0:
			HeroUpgradeWidget->SetLevel1Image(WhiteStar);
			HeroUpgradeWidget->SetLevel2Image(WhiteStar);
			HeroUpgradeWidget->SetLevel3Image(WhiteStar);
			break;
		case 1:
			HeroUpgradeWidget->SetLevel1Image(GoldStar);
			HeroUpgradeWidget->SetLevel2Image(WhiteStar);
			HeroUpgradeWidget->SetLevel3Image(WhiteStar);
			break;
		case 2:
			HeroUpgradeWidget->SetLevel1Image(GoldStar);
			HeroUpgradeWidget->SetLevel2Image(GoldStar);
			HeroUpgradeWidget->SetLevel3Image(WhiteStar);
			break;
		case 3:
			HeroUpgradeWidget->SetLevel1Image(GoldStar);
			HeroUpgradeWidget->SetLevel2Image(GoldStar);
			HeroUpgradeWidget->SetLevel3Image(GoldStar);
			break;
		default:
			UE_LOG(LogClass, Warning, TEXT("TabHUDWidget: Invalid Level"));
			break;
		}
	}
}

void UTabHUDWidget::UpdatePlayerImagesInWorldMap(const TArray<FVector2D>& PlayerPositions, const TArray<float>& PlayerRotations, int32 NumPlayers)
{
	if (BP_WorldMap)
	{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

		for (int32 i = 0; i < NumPlayers; ++i)
		{
			//InterpolatedPositions[i] = FMath::Vector2DInterpTo(InterpolatedPositions[i], PlayerPositions[i], DeltaTime, 10.f);
			//InterpolatedRotations[i] = FMath::RInterpTo(FRotator(0,InterpolatedRotations[i],0), FRotator(0, PlayerRotations[i],0), DeltaTime, 10.f).Yaw;
			//BP_WorldMap->SetPlayerPosition(i, InterpolatedPositions[i]);
			//BP_WorldMap->SetPlayerRotation(i, InterpolatedRotations[i]);
			BP_WorldMap->SetPlayerPosition(i, PlayerPositions[i]);
			BP_WorldMap->SetPlayerRotation(i, PlayerRotations[i]);
		}

		// 인원 수 보내면 인원 수 넘는 애들은 숨김
		for (int32 i = NumPlayers; i < 4; ++i)
		{
			BP_WorldMap->SetPlayerPosition(i, FVector2D::ZeroVector);
			BP_WorldMap->SetPlayerRotation(i, 0.0f);
			UImage* PlayerImages[4] = { BP_WorldMap->Player1Image, BP_WorldMap->Player2Image, BP_WorldMap->Player3Image, BP_WorldMap->Player4Image };
			if (PlayerImages[i])
			{
				PlayerImages[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UTabHUDWidget::UpdatePlayerHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths)
{
	for(int i = 0; i < PlayerMaxHealths.Num(); i++)
		TabUserInfoArray[i]->SetProgress(PlayerCurrentHealths[i] / PlayerMaxHealths[i]);
}

void UTabHUDWidget::UpdatePlayerLevels(const TArray<int>& PlayerLevels)
{
	for(int i = 0; i < PlayerLevels.Num(); i++)
		TabUserInfoArray[i]->SetLevel(PlayerLevels[i]);
}

void UTabHUDWidget::UpdatePlayerExps(const TArray<float>& PlayerExps)
{
	for(int i = 0; i < PlayerExps.Num(); i++)
		TabUserInfoArray[i]->SetExp(PlayerExps[i]);
}

void UTabHUDWidget::UpdatePlayerClasses(const TArray<int>& PlayerClasses)
{
	for(int i = 0; i < PlayerClasses.Num(); i++)
		TabUserInfoArray[i]->SetClass(static_cast<EClassType>(PlayerClasses[i]));
}

void UTabHUDWidget::UpdatePlayerNames(const TArray<FString>& PlayerNames)
{
	for(int i = 0; i < PlayerNames.Num(); i++)
		TabUserInfoArray[i]->SetName(PlayerNames[i]);
}

void UTabHUDWidget::UpdateGameStateKillCount(const TArray<int> PlayerKillCount)
{
	for(int i = 0; i < PlayerKillCount.Num(); i++)
		TabUserBasicStatsArray[i]->SetKillScore(FText::AsNumber(PlayerKillCount[i]));
}

void UTabHUDWidget::UpdateGameStateGivenDamage(const TArray<float> PlayerGivenDamage)
{
	for(int i = 0; i < PlayerGivenDamage.Num(); i++)
		TabUserBasicStatsArray[i]->SetTotalDamage(FText::AsNumber(PlayerGivenDamage[i]));
}

void UTabHUDWidget::UpdateGameStateReceiveDamage(const TArray<float> PlayerReceiveDamage)
{
	for(int i = 0; i < PlayerReceiveDamage.Num(); i++)
		TabUserBasicStatsArray[i]->SetTotalShield(FText::AsNumber(PlayerReceiveDamage[i]));
}

void UTabHUDWidget::UpdateGameStateHeal(const TArray<float> PlayerHeal)
{
	for(int i = 0; i < PlayerHeal.Num(); i++)
		TabUserBasicStatsArray[i]->SetTotalHeal(FText::AsNumber(PlayerHeal[i]));
}
