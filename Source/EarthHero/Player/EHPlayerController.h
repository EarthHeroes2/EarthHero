
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

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

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

	//승언 : InGameHUD를 컨트롤러에 추가
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> InGameHUD;
	UPROPERTY()
	UUserWidget *HUD;
	
	TObjectPtr<ACharacter> ControlledCharacter;

protected:
	void Jump();
	void Shoot();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
