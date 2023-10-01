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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission BLE")
	void Connect(const FString &Name) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission BLE")
	void Disconnect() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission BLE", DisplayName = "Device Name")
	FString AutoConnectBLEDeviceName = "";

	static const TArray<FString> ServiceUUIDs;

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission BLE")
	const TArray<FString> GetServiceUUIDs() const
	{
		return ServiceUUIDs;
	}

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission BLE")
	const FString GetMainServiceUUID() const
	{
		return ServiceUUIDs[0];
	}

	static const FString DeviceNameCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "Ukaton Mission BLE")
	const FString GetDeviceNameCharacteristicUUID() const
	{
		return DeviceNameCharacteristicUUID;
	}

	static const FString DeviceTypeCharacteristicUUID;
	UFUNCTION(BlueprintPure, Category = "Ukaton Mission BLE")
	const FString GetDeviceTypeCharacteristicUUID() const
	{
		return DeviceTypeCharacteristicUUID;
	}

	virtual FString GetAutoConnectDeviceIdentifier() const override
	{
		return AutoConnectBLEDeviceName;
	};

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission BLE")
	void OnCharacteristicRead(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> Data);
};
