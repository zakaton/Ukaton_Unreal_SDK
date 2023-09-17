// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"
#include "Logging/StructuredLog.h"
#include "ByteParser.h"
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
    uint8 Offset = 0;
    while (Offset < Data.Num())
    {
        auto MessageType = (EUkatonUDPMessageType)Data[Offset++];

        UE_LOGFMT(UkatonMissionUDP, Log, "MessageType: {0}", UEnum::GetValueAsString(MessageType));

        switch (MessageType)
        {
        case EUkatonUDPMessageType::BATTERY_LEVEL:
            ParseBatteryLevel(Data, Offset);
            break;
        case EUkatonUDPMessageType::GET_TYPE:
        case EUkatonUDPMessageType::SET_TYPE:
            ParseDeviceType(Data, Offset);
            break;
        case EUkatonUDPMessageType::GET_NAME:
        case EUkatonUDPMessageType::SET_NAME:
            ParseDeviceName(Data, Offset);
            break;
        case EUkatonUDPMessageType::MOTION_CALIBRATION:
            ParseMotionCalibration(Data, Offset);
            break;
        case EUkatonUDPMessageType::GET_SENSOR_DATA_CONFIGURATIONS:
        case EUkatonUDPMessageType::SET_SENSOR_DATA_CONFIGURATIONS:
            SensorDataConfigurationsManager.ParseConfigurations(Data, Offset);
            break;
        case EUkatonUDPMessageType::SENSOR_DATA:
            ParseSensorData(Data, Offset);
            break;
        case EUkatonUDPMessageType::SET_REMOTE_RECEIVE_PORT:
            ParseSetRemoteReceivePortMessage(Data, Offset);
            break;
        default:
            UE_LOGFMT(UkatonMissionUDP, Error, "Uncaught handler for MessageType: {0}", static_cast<uint8>(MessageType));
            break;
        }
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

void AUkatonMissionUDP::ParseSetRemoteReceivePortMessage(const TArray<uint8> &Data, uint8 &Offset)
{
    auto UpdatedInListenPort = ByteParser::GetUint16(Data, Offset);
    Offset += 2;

    if (UpdatedInListenPort == InListenPort)
    {
        UE_LOGFMT(UkatonMissionUDP, Log, "Successfully set InListenPort!");
        bDidSendSetInListenPortMessage = true;
        EmitBytes(RequestInfoMessage);
    }
}

TArray<uint8> AUkatonMissionUDP::PingMessage = {static_cast<uint8>(EUkatonUDPMessageType::PING)};
TArray<uint8> AUkatonMissionUDP::RequestInfoMessage = {static_cast<uint8>(EUkatonUDPMessageType::GET_TYPE), static_cast<uint8>(EUkatonUDPMessageType::GET_NAME)};