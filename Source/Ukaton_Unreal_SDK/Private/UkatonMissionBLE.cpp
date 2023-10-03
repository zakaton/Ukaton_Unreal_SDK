// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionBLE.h"

#define UKATON_MISSION_BLE_UUID_PREFIX "5691eddf-"
#define UKATON_MISSION_BLE_UUID_SUFFIX "-4420-b7a5-bb8751ab5181"
#define UKATON_MISSION_BLE_GENERATE_UUID(val) (UKATON_MISSION_BLE_UUID_PREFIX val UKATON_MISSION_BLE_UUID_SUFFIX)

AUkatonMissionBLE::AUkatonMissionBLE()
{
    bDidReceiveDeviceName = true;
}

const FString AUkatonMissionBLE::MainServiceUUID = UKATON_MISSION_BLE_GENERATE_UUID("0000");
const FString AUkatonMissionBLE::DeviceTypeCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("3001");
const FString AUkatonMissionBLE::DeviceNameCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("4001");
const FString AUkatonMissionBLE::MotionCalibrationCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("5001");
const FString AUkatonMissionBLE::SensorDataConfigurationsCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("6001");
const FString AUkatonMissionBLE::SensorDataCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("6002");
const FString AUkatonMissionBLE::HapticsVibrationCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("d000");

const TArray<FString> AUkatonMissionBLE::ServiceUUIDs = AUkatonMissionBLE::InitializeServiceUUIDs();

const TArray<FString> AUkatonMissionBLE::InitializeServiceUUIDs()
{
    return {MainServiceUUID};
}

void AUkatonMissionBLE::OnCharacteristicRead(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> &Data)
{
    if (ServiceUUID == MainServiceUUID)
    {
        if (CharacteristicUUID == DeviceTypeCharacteristicUUID)
        {
            ParseDeviceType(Data);
        }
    }
}
void AUkatonMissionBLE::OnCharacteristicNotification(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> &Data)
{
    if (ServiceUUID == MainServiceUUID)
    {
        if (CharacteristicUUID == MotionCalibrationCharacteristicUUID)
        {
            ParseMotionCalibration(Data);
        }
        else if (CharacteristicUUID == SensorDataConfigurationsCharacteristicUUID)
        {
            SensorDataConfigurationsManager.ParseConfigurations(Data);
        }
        else if (CharacteristicUUID == SensorDataCharacteristicUUID)
        {
            ParseSensorData(Data);
        }
    }
}

void AUkatonMissionBLE::SetSensorDataConfigurations()
{
    Super::SetSensorDataConfigurations();
    auto &Configurations = SensorDataConfigurationsManager.SerializedConfigurations;
    WriteCharacteristic(MainServiceUUID, SensorDataConfigurationsCharacteristicUUID, Configurations);
}

void AUkatonMissionBLE::RequestVibration()
{
    auto &Vibration = HapticsManager.SerializedVibration;
    WriteCharacteristic(MainServiceUUID, HapticsVibrationCharacteristicUUID, Vibration);
}