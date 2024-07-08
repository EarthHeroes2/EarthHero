
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
	class UInGameHUD *HUD;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	//승언 : 소유 클라이언트 실행 Possess
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

	//승언 : InGameHUD를 컨트롤러에 추가
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> InGameHUD;

	void InitializeHUD();
	FTimerHandle PlayerStateCheckTimerHandle;
	
	TObjectPtr<ACharacter> ControlledCharacter;

protected:
	void Jump();
	void Shoot();
	void Skill();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
