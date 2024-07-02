#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "StatusWidget.generated.h"

UCLASS()
class EARTHHERO_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttackDamageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttackSpeedText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttackSkillDamageText;
    
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttackSkillCooltimeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoveSkillCooltimeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentShieldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthRegenerationRateText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoveSpeedText;

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAttackDamage(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAttackSpeed(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAttackSkillDamage(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAttackSkillCooltime(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetMoveSkillCooltime(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCurrentHealth(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCurrentShield(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetMaxHealth(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetHealthRegenerationRate(FText Value);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetMoveSpeed(FText Value);
};
