// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionBLE.h"

#define UKATON_MISSION_BLE_UUID_PREFIX "5691eddf-"
#define UKATON_MISSION_BLE_UUID_SUFFIX "-4420-b7a5-bb8751ab5181"
#define UKATON_MISSION_BLE_GENERATE_UUID(val) (UKATON_MISSION_BLE_UUID_PREFIX val UKATON_MISSION_BLE_UUID_SUFFIX)

const TArray<FString> AUkatonMissionBLE::ServiceUUIDs = {
    UKATON_MISSION_BLE_GENERATE_UUID("0000")};

const FString AUkatonMissionBLE::DeviceTypeCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("3001");
const FString AUkatonMissionBLE::DeviceNameCharacteristicUUID = UKATON_MISSION_BLE_GENERATE_UUID("4001");

void AUkatonMissionBLE::OnCharacteristicRead()
{
    // FILL
}