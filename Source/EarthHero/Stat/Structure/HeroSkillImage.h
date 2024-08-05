#pragma once
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "HeroSkillImage.generated.h"

USTRUCT(BlueprintType)
struct FHeroSkillImage : public  FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* NormalAttackImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DashImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCoolTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCoolTime;
};
