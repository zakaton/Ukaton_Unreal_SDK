// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"

void AUkatonMissionUDP::Connect(const FString &IPAddress)
{
}
void AUkatonMissionUDP::Disconnect()
{
    // FILL
}

FString AUkatonMissionUDP::GetAutoConnectDeviceIdentifier() const
{
    return DeviceIPAddress;
};

void AUkatonMissionUDP::ParseMessage(TArray<uint8> Data)
{
    // FILL
}