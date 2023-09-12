// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonPressureValues.h"
#include "UkatonPressureData.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	FUkatonPressureValues PressureValues;
};
