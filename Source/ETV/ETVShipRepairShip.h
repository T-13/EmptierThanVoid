// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVShip.h"
#include "ETVShipRepairShip.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API AETVShipRepairShip : public AETVShip
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AETVShipRepairShip();
	void Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, int32 NewRepairPower);

	void InitRandom(FString NewName);

	void InitRandomWithLevel(FString NewName, int32 PowerLvl);

protected:
	// Maximum how much health can be regained when repairing some other Ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|RepairShip")
	int32 RepairPower;

public:

	UFUNCTION()
	void SpawnWeapons();

	// TODO Add methods

};
