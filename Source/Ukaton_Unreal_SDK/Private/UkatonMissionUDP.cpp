// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"
#include "Logging/StructuredLog.h"
#include "UkatonUDPMessageType.h"

DEFINE_LOG_CATEGORY(UkatonMissionUDP);

AUkatonMissionUDP::AUkatonMissionUDP()
{
    SetInListenPortMessage.SetNum(3);
    SetInListenPortMessage[0] = static_cast<uint8>(EUkatonUDPMessageType::SET_REMOTE_RECEIVE_PORT);
}

void AUkatonMissionUDP::Disconnect_Implementation()
{
    bDidSendSetInListenPortMessage = false;
    bDidReceiveDeviceInfo = false;
}

void AUkatonMissionUDP::ParseMessage(const TArray<uint8> &Data)
{
    // FILL
    uint8 Offset = 0;
    while (Offset < Data.Num())
    {
        auto Value = Data[Offset];
        UE_LOGFMT(UkatonMissionUDP, Warning, "Value #{0}: {1}", Offset, Value);
        Offset++;
    }
}

int32 AUkatonMissionUDP::SetInListenPort(int32 NewInListenPort)
{
    if (InListenPort != NewInListenPort)
    {
        InListenPort = NewInListenPort;
        SetInListenPortMessage[1] = static_cast<uint8>(InListenPort & 0xFF);
        SetInListenPortMessage[2] = static_cast<uint8>((InListenPort >> 8) & 0xFF);
    }
    return InListenPort;
}

TArray<uint8> AUkatonMissionUDP::PingMessage = {static_cast<uint8>(EUkatonUDPMessageType::PING)};
TArray<uint8> AUkatonMissionUDP::RequestInfoMessage = {static_cast<uint8>(EUkatonUDPMessageType::GET_NAME), static_cast<uint8>(EUkatonUDPMessageType::GET_TYPE)};