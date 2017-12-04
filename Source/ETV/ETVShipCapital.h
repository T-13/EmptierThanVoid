// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVShip.h"
#include "ETVShipCapital.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API AETVShipCapital : public AETVShip
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AETVShipCapital();
	void Init(FName NewName, int32 HP, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewSize, int32 NewMoveRange, int32 Speed, bool HyperDrive, float NewLaserResistance, int32 Hangars);

protected:
	// If ship has a hyperdrive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Capital")
	bool HasHyperdrive;

	// Affects damage done by laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Capital", meta = (ClampMin = "0.0"))
	float LaserResistance;

	// Number of hangars smaller ships can go into and repair
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Capital", meta = (ClampMin = "0"))
	int32 NumOfHangars;

public:
	// TODO Add methods
	// UseHyperdrive(int32 x, int32 y);
	// RepairShipInHangar(AETVShip ship, int32 hangar);

};
