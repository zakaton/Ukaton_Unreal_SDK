// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"
#include "Logging/StructuredLog.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonMissionUDP, Log, All);

void AUkatonMissionUDP::Connect_Implementation(const FString &TargetDeviceIPAddress)
{
}
void AUkatonMissionUDP::Disconnect_Implementation()
{
}

void AUkatonMissionUDP::ParseMessage(const TArray<uint8> &Data)
{
    // FILL
}