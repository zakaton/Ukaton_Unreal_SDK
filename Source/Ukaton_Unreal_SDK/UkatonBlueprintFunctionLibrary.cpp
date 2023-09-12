// Copyright (c) 2023 Zack Qattan

#include "UkatonBlueprintFunctionLibrary.h"
#include "Engine/Engine.h"

void UUkatonBlueprintFunctionLibrary::OnUDPMessage(const TArray<uint8> &ByteArray)
{
    FString DebugString = "Byte Values: ";

    for (uint8 ByteValue : ByteArray)
    {
        // Append each byte value to the debug string
        DebugString += FString::Printf(TEXT("%d, "), ByteValue);
    }

    // Add the debug string to the screen
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugString);
}