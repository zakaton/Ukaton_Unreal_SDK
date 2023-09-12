// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonPressureDataType : uint8
{
	PRESSURE_SINGLE_BYTE UMETA(DisplayName = "Pressure (single-byte precision)"),
	PRESSURE_DOUBLE_BYTE UMETA(DisplayName = "Pressure (double-byte precision)"),
	CENTER_OF_MASS UMETA(DisplayName = "Center of Mass"),
	MASS UMETA(DisplayName = "Mass"),
	HEEL_TO_TOE UMETA(DisplayName = "Heel-to-Toe"),
	COUNT UMETA(Hidden)
};