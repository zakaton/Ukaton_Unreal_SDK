// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "UkatonPressureValue.h"
#include "UkatonDeviceType.h"
#include "UkatonPressureValues.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureValues
{
	GENERATED_BODY()

	static const uint8 NumberOfPressureSensors = 16;
	static const FVector2D PressurePositions[NumberOfPressureSensors];

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Values")
	TArray<FUkatonPressureValue> PressureValues;

	FUkatonPressureValues()
	{
		PressureValues.SetNum(NumberOfPressureSensors);
	};

	
};