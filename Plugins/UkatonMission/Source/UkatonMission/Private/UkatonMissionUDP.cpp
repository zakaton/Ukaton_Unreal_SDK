// Copyright (c) 2023 Zack Qattan

#include "UkatonMissionUDP.h"
#include "Logging/StructuredLog.h"
#include "ByteParser.h"
#include "GenericPlatform/GenericPlatformTime.h"
#include "UkatonUDPMessageType.h"

DEFINE_LOG_CATEGORY(LogUkatonMissionUDP);

AUkatonMissionUDP::AUkatonMissionUDP()
{
    SetInListenPortMessage.SetNum(3);
    SetInListenPortMessage[0] = static_cast<uint8>(EUkatonUDPMessageType::SET_REMOTE_RECEIVE_PORT);
}

void AUkatonMissionUDP::Disconnect_Implementation()
{
    Super::Disconnect();
}

void AUkatonMissionUDP::SetIsConnected(bool bNewIsConnected)
{
    if (bIsConnected != bNewIsConnected)
    {
        if (!bNewIsConnected)
        {
            bDidSendSetInListenPortMessage = false;
        }
        Super::SetIsConnected(bNewIsConnected);
    }
}

void AUkatonMissionUDP::ParseMessage(const TArray<uint8> &Data)
{
    uint8 Offset = 0;
    while (Offset < Data.Num())
    {
        auto MessageType = (EUkatonUDPMessageType)Data[Offset++];

        UE_LOGFMT(LogUkatonMissionUDP, Log, "MessageType: {0}", UEnum::GetValueAsString(MessageType));

        switch (MessageType)
        {
        case EUkatonUDPMessageType::PING:
            break;
        case EUkatonUDPMessageType::BATTERY_LEVEL:
            ParseBatteryLevel(Data, Offset);
            break;
        case EUkatonUDPMessageType::GET_TYPE:
        case EUkatonUDPMessageType::SET_TYPE:
            ParseDeviceType(Data, Offset);
            break;
        case EUkatonUDPMessageType::GET_NAME:
        case EUkatonUDPMessageType::SET_NAME:
        {
            uint8 NameLength = Data[Offset++];
            uint8 FinalOffset = Offset + NameLength;
            ParseDeviceName(Data, Offset, FinalOffset);
        }
        break;
        case EUkatonUDPMessageType::MOTION_CALIBRATION:
            ParseMotionCalibration(Data, Offset);
            break;
        case EUkatonUDPMessageType::GET_SENSOR_DATA_CONFIGURATIONS:
        case EUkatonUDPMessageType::SET_SENSOR_DATA_CONFIGURATIONS:
            SensorDataConfigurationsManager.ParseConfigurations(Data, Offset);
            break;
        case EUkatonUDPMessageType::SENSOR_DATA:
        {
            uint8 DataSize = Data[Offset++];
            uint8 FinalOffset = Offset + DataSize;
            ParseSensorData(Data, Offset, FinalOffset);
        }
        break;
        case EUkatonUDPMessageType::SET_REMOTE_RECEIVE_PORT:
            ParseSetRemoteReceivePortMessage(Data, Offset);
            break;
        default:
            UE_LOGFMT(LogUkatonMissionUDP, Error, "Uncaught handler for MessageType: {0}", static_cast<uint8>(MessageType));
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
        UE_LOGFMT(LogUkatonMissionUDP, Log, "Successfully set InListenPort!");
        bDidSendSetInListenPortMessage = true;
        EmitBytes(RequestInfoMessage);
    }
}

void AUkatonMissionUDP::SetSensorDataConfigurations()
{
    Super::SetSensorDataConfigurations();
    auto &Configurations = SensorDataConfigurationsManager.SerializedConfigurations;
    auto Size = Configurations.Num();
    Configurations.Insert(static_cast<uint8>(EUkatonUDPMessageType::SET_SENSOR_DATA_CONFIGURATIONS), 0);
    Configurations.Insert(static_cast<uint8>(Size), 1);
    EmitBytes(Configurations);
}

const TArray<uint8> &AUkatonMissionUDP::GetPingMessage()
{
    if (bDidSendSetInListenPortMessage && SensorDataConfigurationsManager.IsConfigurationNonZero)
    {
        auto CurrentTime = FGenericPlatformTime::Seconds();
        if (CurrentTime - SensorDataManager.LastTimeReceivedSensorData > 2)
        {
            UE_LOGFMT(LogUkatonMissionUDP, Log, "Didn't receive data in 2+ seconds");
            SetIsConnected(false);
        }
    }

    return bDidSendSetInListenPortMessage ? PingMessage : SetInListenPortMessage;
};

TArray<uint8> AUkatonMissionUDP::PingMessage = {static_cast<uint8>(EUkatonUDPMessageType::PING)};
TArray<uint8> AUkatonMissionUDP::RequestInfoMessage = {static_cast<uint8>(EUkatonUDPMessageType::GET_TYPE), static_cast<uint8>(EUkatonUDPMessageType::GET_NAME)};

void AUkatonMissionUDP::RequestVibration()
{
    auto &Vibration = HapticsManager.SerializedVibration;
    auto Size = Vibration.Num();
    Vibration.Insert(static_cast<uint8>(EUkatonUDPMessageType::VIBRATION), 0);
    Vibration.Insert(static_cast<uint8>(Size), 1);
    EmitBytes(Vibration);
}