// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionBLE.h"

#define UKATON_MISSION_BLE_UUID_PREFIX "5691eddf-"
#define UKATON_MISSION_BLE_UUID_SUFFIX "-4420-b7a5-bb8751ab5181"
#define UKATON_MISSION_BLE_GENERATE_UUID(val) (UKATON_MISSION_BLE_UUID_PREFIX val UKATON_MISSION_BLE_UUID_SUFFIX)

const FString AUkatonMissionBLE::MainServiceUUID = UKATON_MISSION_BLE_GENERATE_UUID("0000");
const FString AUkatonMissionBLE::DeviceTypeCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("3001");
const FString AUkatonMissionBLE::DeviceNameCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("4001");

const TArray<FString> AUkatonMissionBLE::ServiceUUIDs = AUkatonMissionBLE::InitializeServiceUUIDs();

const TArray<FString> AUkatonMissionBLE::InitializeServiceUUIDs()
{
    return {MainServiceUUID};
}

void AUkatonMissionBLE::OnCharacteristicRead(const FString &ServiceUUID, const FString &CharacteristicUUID, const TArray<uint8> Data)
{
    // FILL
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("FUCK YES"));
}

bool AUkatonMissionBLE::OnBleDevice(UObject *InObject)
{
    IBleDeviceInterface *BleDeviceInterface = Cast<IBleDeviceInterface>(InObject);
    if (BleDeviceInterface)
    {
        auto Name = BleDeviceInterface->GetDeviceName();
        if (Name == GetAutoConnectDeviceIdentifier())
        {
            if (BleDeviceObject)
            {
                IBleDeviceInterface *MyBleDeviceInterface = Cast<IBleDeviceInterface>(BleDeviceObject);
                if (MyBleDeviceInterface->GetDeviceId() != BleDeviceInterface->GetDeviceId())
                {
                    return false;
                }
            }
            BleDeviceObject = InObject;
            BindEventsToBleDevice();
            return true;
        }
    }
    return false;
}

void AUkatonMissionBLE::BindEventsToBleDevice()
{
    IBleDeviceInterface *BleDeviceInterface = Cast<IBleDeviceInterface>(BleDeviceObject);
    if (BleDeviceInterface)
    {
        BleDeviceInterface->OnReadDelegate.BindUFunction(this, "OnCharacteristicRead");
    }
};

void AUkatonMissionBLE::ReadDeviceTypeCharacteristic()
{
    IBleDeviceInterface *BleDeviceInterface = Cast<IBleDeviceInterface>(BleDeviceObject);
    if (BleDeviceInterface)
    {
        BleDeviceInterface->ReadCharacteristic(MainServiceUUID, DeviceTypeCharacteristicUUID);
    }
};