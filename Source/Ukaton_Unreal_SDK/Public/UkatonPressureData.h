// Fill out your copyright notice in the Description page of Project Settings.

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
