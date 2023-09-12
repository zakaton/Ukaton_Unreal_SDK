// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "UkatonPressureDataType.h"
#include "UkatonPressureValues.h"
#include "UkatonPressureData.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	FUkatonPressureValues PressureValues;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	FVector2D CenterOfMass;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	float Mass;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Data")
	float HeelToToe;

	uint8 ParseData(const TArray<uint8> &Data, uint8 Offset = 0);

protected:
	static const TMap<EUkatonPressureDataType, float> ScalarMap;
};
