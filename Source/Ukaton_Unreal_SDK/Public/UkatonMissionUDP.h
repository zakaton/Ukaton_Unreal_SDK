// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionUDP.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(UkatonMissionUDP, Log, All);

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionUDP : public AUkatonMissionBase
{
	GENERATED_BODY()

public:
	AUkatonMissionUDP();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission UDP")
	void Connect(const FString &IPAddress) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ukaton Mission UDP")
	void Disconnect() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ukaton Mission UDP", DisplayName = "IP Address")
	FString AutoConnectDeviceIPAddress = "0.0.0.0";

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission UDP")
	void ParseMessage(const TArray<uint8> &Data);

	virtual FString GetAutoConnectDeviceIdentifier() const override
	{
		return AutoConnectDeviceIPAddress;
	};

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	static const int32 GetSendPort()
	{
		return SendPort;
	};

	UPROPERTY(BlueprintReadWrite, Category = "Ukaton Mission UDP")
	bool bDidReceiveDeviceInfo;
	UPROPERTY(BlueprintReadWrite, Category = "Ukaton Mission UDP")
	bool bDidSendSetInListenPortMessage;

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	static const TArray<uint8> &GetPingMessage()
	{
		return PingMessage;
	};

	UFUNCTION(BlueprintPure, Category = "Ukaton Mission UDP")
	static const TArray<uint8> &GetRequestInfoMessage()
	{
		return RequestInfoMessage;
	};

	UPROPERTY(BlueprintReadOnly, Category = "Ukaton Mission UDP")
	TArray<uint8> SetInListenPortMessage;

private:
	static const int32 SendPort = 9999;

	static TArray<uint8> PingMessage;
	static TArray<uint8> RequestInfoMessage;
};
