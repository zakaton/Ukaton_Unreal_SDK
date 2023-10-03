// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionBLE.generated.h"

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionBLE : public AUkatonMissionBase
{
	GENERATED_BODY()

public:
	AUkatonMissionBLE();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission BLE")
	void Connect(const FString &Name) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission BLE")
	void Disconnect() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission BLE", DisplayName = "Device Name")
	FString AutoConnectBLEDeviceName = "";

	static const FString MainServiceUUID;
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Ukaton Mission BLE")
	const FString &GetMainServiceUUID() const
	{
		return MainServiceUUID;
	}

	static const TArray<FString> ServiceUUIDs;
	UFUNCTION(BlueprintPure, Category = "Ukaton Mission BLE")
	const TArray<FString> GetServiceUUIDs() const
	{
		return ServiceUUIDs;
	}
	static const TArray<FString> InitializeServiceUUIDs();

	static const FString DeviceTypeCharacteristicUUID;
	static const FString DeviceNameCharacteristicUUID;

	static const FString MotionCalibrationCharacteristicUUID;
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Ukaton Mission BLE")
	const FString &GetMotionCalibrationCharacteristicUUID() const
	{
		return MotionCalibrationCharacteristicUUID;
	}

	static const FString SensorDataConfigurationsCharacteristicUUID;
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Ukaton Mission BLE")
	const FString &GetSensorDataConfigurationsCharacteristicUUID() const
	{
		return SensorDataConfigurationsCharacteristicUUID;
	}

	static const FString SensorDataCharacteristicUUID;
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Ukaton Mission BLE")
	const FString &GetSensorDataCharacteristicUUID() const
	{
		return SensorDataCharacteristicUUID;
	}

	static const FString HapticsVibrationCharacteristicUUID;

	virtual FString GetAutoConnectDeviceIdentifier() const override
	{
		return AutoConnectBLEDeviceName;
	};

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission BLE")
	void OnCharacteristicRead(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> &Data);
	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission BLE")
	void OnCharacteristicNotification(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> &Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ukaton Mission BLE")
	void ReadCharacteristic(const FString &ServiceUUID, const FString &CharacteristicUUID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ukaton Mission BLE")
	void WriteCharacteristic(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> &Data);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ukaton Mission BLE")
	void SubscribeToCharacteristic(const FString &ServiceUUID, const FString &CharacteristicUUID);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ukaton Mission BLE")
	void UnsubscribeFromCharacteristic(const FString &ServiceUUID, const FString &CharacteristicUUID);

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission BLE")
	void ReadDeviceTypeCharacteristic()
	{
		ReadCharacteristic(MainServiceUUID, DeviceTypeCharacteristicUUID);
	}

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission BLE")
	void ReadDeviceNameCharacteristic()
	{
		ReadCharacteristic(MainServiceUUID, DeviceNameCharacteristicUUID);
	}

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission BLE")
	virtual void SetSensorDataConfigurations() override;

private:
	void RequestVibration() override;
};
