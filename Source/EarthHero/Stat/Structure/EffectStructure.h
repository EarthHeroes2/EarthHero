#pragma once
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "EffectStructure.generated.h"

USTRUCT(BlueprintType)
struct FEffectStructure : public  FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EffectType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* EffectImage = nullptr;
};
