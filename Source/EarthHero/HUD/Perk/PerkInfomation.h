// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EARTHHERO_API PerkInfomation
{
public:
	int NeedPoint[50] = {1, 1, 1, 2, 2,
						 1, 1, 2, 2, 3,
						 1, 2, 3, 3, 4,
						 1, 1, 2, 2, 2,
						 1, 2, 3, 4, 5,
						 2, 2, 2, 4, 4,
						 1, 2, 2, 4, 5,
						 1, 2, 2, 3, 3,
						 2, 2, 2, 3, 5,
						 1, 1, 3, 5, 5};

	int NumOfLevels = 10;
	int NumOfPerkPerLevel = 5;
	int NumOfPerks = NumOfLevels * NumOfPerkPerLevel;

	int GetPoints(int Level);
};
