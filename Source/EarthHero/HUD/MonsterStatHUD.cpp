// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatHUD.h"
#include "Structure/Status.h"

bool UMonsterStatHUD::Initialize()
{
	Super::Initialize();
	
	StatusArray.Add(FStatus(0, Status));
	StatusArray.Add(FStatus(0, Status_1));
	StatusArray.Add(FStatus(0, Status_2));
	StatusArray.Add(FStatus(0, Status_3));
	StatusArray.Add(FStatus(0, Status_4));
	StatusArray.Add(FStatus(0, Status_5));
	StatusArray.Add(FStatus(0, Status_6));
	StatusArray.Add(FStatus(0, Status_7));
	return true;
}
