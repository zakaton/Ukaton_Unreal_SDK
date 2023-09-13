// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Math/Vector2D.h"
#include "UkatonPressureValue.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureValue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Value")
	int32 RawValue;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Value")
	float WeightedValue;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Value")
	float NormalizedValue;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Pressure Value")
	FVector2D Position;
};
