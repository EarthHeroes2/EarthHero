#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscMenu.generated.h"

UCLASS()
class EARTHHERO_API UEscMenu : public UUserWidget
{
	GENERATED_BODY()

	TSubclassOf<UUserWidget> OptionsWidgetClass;
	UUserWidget* OptionsWidget;
public:
	UEscMenu(const FObjectInitializer &ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveBtn;
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnSettingsClicked();

	UFUNCTION()
	void OnLeaveClicked();
};
