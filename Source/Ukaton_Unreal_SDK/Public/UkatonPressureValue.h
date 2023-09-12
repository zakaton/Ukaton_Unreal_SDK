// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UkatonPressureValue.generated.h"

USTRUCT(BlueprintType)
struct FUkatonPressureValue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	float RawValue;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	float WeightedValue;

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton")
	float NormalizedValue;
};
