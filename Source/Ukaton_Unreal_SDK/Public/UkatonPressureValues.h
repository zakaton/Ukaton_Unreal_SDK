// // Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonPressureValue.h"
#include "UkatonPressureValues.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureValues
{
	GENERATED_BODY()

	static const uint8 NumberOfPressureSensors = 16;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	TArray<FUkatonPressureValue> PressureValues;

	FUkatonPressureValues()
	{
		PressureValues.SetNum(NumberOfPressureSensors);
	};
};
