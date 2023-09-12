// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonSensorTypeEnum : uint8
{
	MOTION UMETA(DisplayName = "Motion Sensor"),
	PRESSURE UMETA(DisplayName = "Pressure Sensor"),
	COUNT UMETA(Hidden)
};