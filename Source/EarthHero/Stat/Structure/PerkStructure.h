#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PerkStructure.generated.h"

USTRUCT(BlueprintType)
struct FPerkStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PerkName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;
};
