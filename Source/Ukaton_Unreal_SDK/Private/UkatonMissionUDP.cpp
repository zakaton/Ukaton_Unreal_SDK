// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonMissionUDP, Log, All);

void AUkatonMissionUDP::Connect(const FString &IPAddress)
{
    if (bIsConnecting)
    {
        Disconnect();
    }

    bIsConnecting = true;
    // FILL
}
void AUkatonMissionUDP::Disconnect()
{
    // FILL
    if (bIsConnecting)
    {
        bIsConnecting = false;
    }
    else if (bIsConnected)
    {
        bIsConnected = false;
    }
}

FString AUkatonMissionUDP::GetAutoConnectDeviceIdentifier() const
{
    return DeviceIPAddress;
};

void AUkatonMissionUDP::ParseMessage(const TArray<uint8> &Data)
{
    // FILL
}