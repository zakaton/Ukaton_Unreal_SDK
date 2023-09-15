// Copyright (c) 2023 Zack Qattan

#pragma once

#include "CoreMinimal.h"
#include "UkatonMissionBase.h"
#include "UkatonMissionBLE.generated.h"

UCLASS()
class UKATON_UNREAL_SDK_API AUkatonMissionBLE : public AUkatonMissionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission")
	virtual void Connect(const FString &BLEDeviceName) override;

	UFUNCTION(BlueprintCallable, Category = "Ukaton Mission")
	virtual void Disconnect() override;
};
