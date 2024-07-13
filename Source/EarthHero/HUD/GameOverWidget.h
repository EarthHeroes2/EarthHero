// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddChatMessage(const FText& Text);

private:
	virtual bool Initialize() override;

	UFUNCTION()
	void ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void ExitClicked();


	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Chat_Scr;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Chat_Etb;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Exit_Btn;
};


