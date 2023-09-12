// Copyright (c) 2023 Zack Qattan

#include "UkatonBaseMission.h"

// Sets default values
AUkatonBaseMission::AUkatonBaseMission()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsConnected = false;
}

// Called when the game starts or when spawned
void AUkatonBaseMission::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUkatonBaseMission::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}