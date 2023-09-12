// // Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UkatonBlueprintFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class UKATON_UNREAL_SDK_API UUkatonBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Parses all incoming UDP messages */
	UFUNCTION(BlueprintCallable, Category = "Ukaton")
	static void OnUDPMessage(const TArray<uint8> &ByteArray);
};
