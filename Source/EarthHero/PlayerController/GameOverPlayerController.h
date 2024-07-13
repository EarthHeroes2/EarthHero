// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameOverPlayerController.generated.h"

class UGameOverWidget;
/**
 * 
 */
UCLASS()
class EARTHHERO_API AGameOverPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	AGameOverPlayerController();

	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UGameOverWidget* GameOverWidget;

public:
	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FText& Text);
	UFUNCTION(Client, Reliable)
	void Client_SendChatMessage(const FText& Text);
	
};
