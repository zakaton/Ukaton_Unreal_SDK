// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"
#include "Logging/StructuredLog.h"
#include "UkatonUDPMessageType.h"

DEFINE_LOG_CATEGORY(UkatonMissionUDP);

AUkatonMissionUDP::AUkatonMissionUDP()
{
    // FILL
}

void AUkatonMissionUDP::ParseMessage(const TArray<uint8> &Data)
{
    // FILL
}

TArray<uint8> AUkatonMissionUDP::PingMessage = {static_cast<uint8>(EUkatonUDPMessageType::PING)};
TArray<uint8> AUkatonMissionUDP::RequestInfoMessage = {static_cast<uint8>(EUkatonUDPMessageType::GET_NAME), static_cast<uint8>(EUkatonUDPMessageType::GET_TYPE)};