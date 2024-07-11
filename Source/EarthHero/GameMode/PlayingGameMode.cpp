// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameMode.h"

//심레스 트래블 이후 새로운 플레이어 컨트롤러 생김?
void APlayingGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	APlayerController* NewPlayerController = Cast<APlayerController>(NewController);

	
	
}