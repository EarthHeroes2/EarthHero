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

	UFUNCTION(Client, Reliable)
	void Client_EnableInput();

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
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ChatAction;

	//07 13 히어로 업그레이드 선택 맵핑 액션 추가
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SelectHUAction_1;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SelectHUAction_2;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SelectHUAction_3;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> InGameHUD;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> TabHUDClass;
	
	void InitializeHUD();

	UPROPERTY()
	class AEHPlayerState *MyPlayerState;

	UFUNCTION(Server, Reliable)
	void Server_PlayerControllerReady();

	
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
	void FocusChatBox();
	void SelectHU_1();
	void SelectHU_2();
	void SelectHU_3();
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FText& Text);

	UFUNCTION(Client, Reliable)
	void Client_SendChatMessage(const FText& Text);
};
