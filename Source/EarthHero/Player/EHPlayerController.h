#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "EHPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class EARTHHERO_API AEHPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEHPlayerController();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	class UInGameHUD* HUD;

	UPROPERTY()
	class UTabHUDWidget* TabHUD;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client, Reliable)
	void ClientPossess();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HeroContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CamAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SkillAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> TabAction;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> InGameHUD;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> TabHUDClass;

	void InitializeHUD();
	FTimerHandle PlayerStateCheckTimerHandle;

	TObjectPtr<ACharacter> ControlledCharacter;

protected:
	void Jump();
	void Shoot();
	void Skill();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ShowTabHUD(); // Add the function to show the TabHUD
	void HideTabHUD(); // Add the function to hide the TabHUD
};
