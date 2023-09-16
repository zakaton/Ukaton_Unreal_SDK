// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUkatonUDPMessageType : uint8
{
	PING UMETA(DisplayName = "Ping"),
	BATTERY_LEVEL UMETA(DisplayName = "Battery Level"),

	GET_TYPE UMETA(DisplayName = "Get Type"),
	SET_TYPE UMETA(DisplayName = "Set Type"),

	GET_NAME UMETA(DisplayName = "Get Name"),
	SET_NAME UMETA(DisplayName = "Set Name"),

	MOTION_CALIBRATION UMETA(DisplayName = "Motion Calibration"),

	GET_SENSOR_DATA_CONFIGURATIONS UMETA(DisplayName = "Get Sensor Data Configurations"),
	SET_SENSOR_DATA_CONFIGURATIONS UMETA(DisplayName = "Set Sensor Data Configurations"),

	SENSOR_DATA UMETA(DisplayName = "Sensor Data"),

	VIBRATION UMETA(DisplayName = "Vibration"),

	SET_REMOTE_RECEIVE_PORT UMETA(DisplayName = "Set Remote Receive Port")
};